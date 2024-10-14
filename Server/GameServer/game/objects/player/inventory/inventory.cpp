#include "pch.h"
#include "inventory.h"

#include "data/item_provider.h"

#include "database/db_bind.h"
#include "database/db_connection_pool.h"

#include "game/objects/player/item.h"

#include <pugixml.hpp>

Inventory::Inventory() {
  _removed_items.reserve(15);
}

bool Inventory::AddItem(const InventoryType type, int32_t pos, const std::shared_ptr<Item>& item) {
  if (pos < 0 || pos >= kMaxSlot) {
    return false;
  }

  std::unique_lock lock(_items_mutex[type]);
  return _items[type].emplace(pos, item).second;
}

bool Inventory::AddItem(const InventoryType type, const std::shared_ptr<ItemTemplate>& item_template, int32_t quantity) {
  if (!CanHold(type, item_template->GetId(), quantity, item_template->GetSlotMax())) {
    return false;
  }

  std::unique_lock lock(_items_mutex[type]);

  if (type == kEquip) {
    bool success = true;

    for (int i = 0; i < quantity; i++) {
      for (int32_t pos = 0; pos < kMaxSlot; ++pos) {
        if (!_items[type].contains(pos)) {
          const auto item = std::make_shared<Item>(item_template, 1);
          success &= _items[type].emplace(pos, item).second;
        }
      }
    }

    return success;
  }

  const auto max_count = item_template->GetSlotMax();

  for (const auto& inventory_item : _items[type] | std::views::values) {
    if (quantity == 0) {
      return true;
    }

    if (inventory_item->GetId() != item_template->GetId()) {
      continue;
    }

    const auto current_count = inventory_item->GetQuantity();
    const auto diff = max_count - current_count;

    if (diff > quantity) {
      inventory_item->SetQuantity(current_count + quantity);
      return true;
    }

    inventory_item->SetQuantity(max_count);
    quantity = max(0, quantity - diff);
  }

  const auto slot_count = quantity / max_count + (quantity % max_count != 0);
  bool success = true;

  for (int32_t i = 0; i < slot_count; ++i) {
    for (int32_t j = 0; j < kMaxSlot; ++j) {
      if (!_items[type].contains(j)) {
        const auto add_count = min(quantity, max_count);
        const auto item = std::make_shared<Item>(item_template, add_count);

        success &= _items[type].emplace(j, item).second;
      }
    }
  }

  return success;
}

std::optional<std::shared_ptr<Item>> Inventory::GetItem(const InventoryType type, const int32_t pos) const {
  if (pos < 0 || pos >= kMaxSlot) {
    return std::nullopt;
  }

  std::shared_lock lock(_items_mutex[type]);
  const auto it = _items[type].find(pos);

  if (it == _items[type].end()) {
    return std::nullopt;
  }

  return it->second;
}

void Inventory::SwapItem(const InventoryType type, const int32_t src, const int32_t dst) {
  if (src < 0 || src >= kMaxSlot || dst < 0 || dst >= kMaxSlot) {
    return;
  }

  if (src == dst) {
    return;
  }

  std::unique_lock lock(_items_mutex[type]);
  std::unique_lock remove_lock(_removed_items_mutex);

  const auto src_it = _items[type].find(src);
  const auto dst_it = _items[type].find(dst);

  if (src_it == _items[type].end()) {
    return;
  }

  if (dst_it == _items[type].end()) {
    _items.at(type).emplace(dst, src_it->second);
    _items.at(type).erase(src);
    _removed_items.emplace_back(type, src);
  } else {
    if (src_it->second->GetId() != dst_it->second->GetId()) {
      std::swap(src_it->second, dst_it->second);
      return;
    }

    const auto max_count = src_it->second->GetItemTemplate()->GetSlotMax();

    if (src_it->second->GetQuantity() + dst_it->second->GetQuantity() > max_count) {
      const auto diff = src_it->second->GetQuantity() + dst_it->second->GetQuantity() - max_count;
      dst_it->second->SetQuantity(max_count);
      src_it->second->SetQuantity(diff);
    } else {
      dst_it->second->SetQuantity(src_it->second->GetQuantity() + dst_it->second->GetQuantity());
      _items[type].erase(src);
      _removed_items.emplace_back(type, src);
    }
  }
}

bool Inventory::RemoveItem(const InventoryType type, const int32_t pos) {
  if (pos < 0 || pos >= kMaxSlot) {
    return false;
  }

  std::unique_lock lock(_items_mutex[type]);

  if (_items[type].erase(pos)) {
    std::shared_lock remove_lock(_removed_items_mutex);

    _removed_items.emplace_back(type, pos);
    return true;
  }

  return false;
}

bool Inventory::RemoveItem(const InventoryType type, const int32_t pos, const int32_t quantity) {
  if (quantity <= 0) {
    return false;
  }

  std::unique_lock lock(_items_mutex[type]);
  const auto it = _items[type].find(pos);

  if (it == _items[type].end()) {
    return false;
  }

  const auto current_count = it->second->GetQuantity();

  if (current_count <= quantity) {
    if (_items[type].erase(pos)) {
      std::shared_lock remove_lock(_removed_items_mutex);

      _removed_items.emplace_back(type, pos);
      return true;
    }

    return false;
  }

  it->second->SetQuantity(current_count - quantity);
  return true;
}

std::map<int32_t, std::shared_ptr<Item>> Inventory::GetAllItems(const InventoryType type) const {
  std::shared_lock lock(_items_mutex[type]);
  std::map<int32_t, std::shared_ptr<Item>> items;

  for (const auto& item : _items[type]) {
    items.emplace(item.first, item.second);
  }

  return items;
}

bool Inventory::CanHold(const InventoryType type, const uint32_t id, int32_t quantity, const int32_t max_count) const {
  std::shared_lock lock(_items_mutex[type]);

  if (type == kEquip) {
    return _items[type].size() + quantity <= kMaxSlot;
  }

  for (const auto& inventory_item : _items[type] | std::views::values) {
    if (inventory_item->GetId() != id) {
      continue;
    }

    const auto current_count = inventory_item->GetQuantity();
    const auto diff = max_count - current_count;
    quantity -= diff;

    if (quantity <= 0) {
      return true;
    }
  }

  return false;
}

bool Inventory::TryLoadFromDb(int32_t player_id) {
  bool success = true;
  std::unique_lock equip_lock(_items_mutex[kEquip]);
  std::unique_lock use_lock(_items_mutex[kUse]);
  std::unique_lock etc_lock(_items_mutex[kEtc]);
  std::unique_lock equipped_lock(_items_mutex[kEquipped]);
  std::unique_lock remove_lock(_removed_items_mutex);

  _removed_items.clear();

  for (auto& items : _items) {
    items.clear();
  }

  if (const auto connection = DbConnectionPool::GetInstance().GetConnection()) {
    DbBind<1, 4> bind(*connection, L"{CALL dbo.spLoadInventory(?)}");
    bind.BindParam(0, player_id);

    int32_t item_id = 0;
    bind.BindCol(0, item_id);
    int32_t type = 0;
    bind.BindCol(1, type);
    int32_t index = 0;
    bind.BindCol(2, index);
    int32_t quantity = 0;
    bind.BindCol(3, quantity);

    if (bind.Execute()) {
      do {
        int16_t count;
        bind.GetResultColumnCount(&count);

        if (count != 4) {
          success = false;
          break;
        }

        while (bind.Fetch()) {
          if (item_id == 0 || quantity == 0) {
            continue;
          }

          const auto item_template = ItemProvider::GetInstance().GetItem(static_cast<uint32_t>(item_id));

          if (!item_template.has_value()) {
            success = false;
            break;
          }

          const auto item = std::make_shared<Item>(*item_template, quantity);

          if (!_items[type].emplace(index, item).second) {
            success = false;
            break;
          }
        }
      } while (bind.GetMoreResult() != SQL_NO_DATA);
    }

    DbConnectionPool::GetInstance().ReleaseConnection(connection);
  }

  return success;
}

bool Inventory::TrySaveToDb(int32_t player_id) {
  const auto xml = SerializeToXml();

  bool success = false;

  if (const auto connection = DbConnectionPool::GetInstance().GetConnection()) {
    DbBind<2, 1> bind(*connection, L"{CALL dbo.spSaveItemsXml(?, ?)}");

    bind.BindParam(0, player_id);
    bind.BindParam(1, xml);

    int temp = 0;
    bind.BindCol(0, temp);

    if (bind.Execute()) {
      do {
        int16_t count;
        bind.GetResultColumnCount(&count);

        if (count == 0) {
          break;
        }

        while (bind.Fetch()) {
          success = true;
        }
      } while (bind.GetMoreResult() != SQL_NO_DATA);
    }

    DbConnectionPool::GetInstance().ReleaseConnection(connection);
  }

  return success;
}

std::wstring Inventory::SerializeToXml() {
  pugi::xml_document doc;
  auto root = doc.append_child("ms");
  auto children = root.append_child("items");
  auto removed = root.append_child("removed");

  std::shared_lock remove_lock(_removed_items_mutex);

  for (const auto& [type, pos] : _removed_items) {
    auto child = removed.append_child("item");
    child.append_attribute("type").set_value(type);
    child.append_attribute("pos").set_value(pos);
  }

  _removed_items.clear();

  for (int i = 0; i < kEnd; i++) {
    std::shared_lock lock(_items_mutex[i]);

    if (_items[i].empty()) {
      continue;
    }

    for (const auto& [key, entry] : _items[i]) {
      auto child = children.append_child("item");
      child.append_attribute("type").set_value(i);
      child.append_attribute("pos").set_value(key);
      child.append_attribute("id").set_value(entry->GetId());
      child.append_attribute("quantity").set_value(entry->GetQuantity());
    }
  }

  std::wstringstream stream;
  doc.save(stream);

  return stream.str();
}
