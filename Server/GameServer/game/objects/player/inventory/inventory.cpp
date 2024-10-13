#include "pch.h"
#include "inventory.h"

#include "database/db_bind.h"
#include "database/db_connection_pool.h"

#include "game/objects/player/item.h"

bool Inventory::AddItem(const InventoryType type, int32_t pos, const std::shared_ptr<Item>& item) {
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

  for (auto& [_, inventory_item] : _items[type]) {
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
  std::shared_lock lock(_items_mutex[type]);
  const auto it = _items[type].find(pos);

  if (it == _items[type].end()) {
    return std::nullopt;
  }

  return it->second;
}

void Inventory::SwapItem(const InventoryType type, const int32_t src, const int32_t dst) {
  std::unique_lock lock(_items_mutex[type]);

  const auto src_it = _items[type].find(src);
  const auto dst_it = _items[type].find(dst);

  if (src_it == _items[type].end() || dst_it == _items[type].end()) {
    return;
  }

  std::swap(src_it->second, dst_it->second);
}

bool Inventory::RemoveItem(const InventoryType type, const int32_t pos) {
  std::unique_lock lock(_items_mutex[type]);
  return _items[type].erase(pos);
}

std::vector<std::shared_ptr<Item>> Inventory::GetAllItems(const InventoryType type) const {
  std::shared_lock lock(_items_mutex[type]);
  std::vector<std::shared_ptr<Item>> items;

  for (const auto& [_, item] : _items[type]) {
    items.push_back(item);
  }

  return items;
}

bool Inventory::CanHold(const InventoryType type, const uint32_t id, int32_t quantity, const int32_t max_count) const {
  std::shared_lock lock(_items_mutex[type]);

  if (type == kEquip) {
    return _items[type].size() + quantity <= kMaxSlot;
  }

  for (const auto& [_, inventory_item] : _items[type]) {
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

  if (const auto connection = DbConnectionPool::GetInstance().GetConnection()) {
    DbBind<1, 3> bind(*connection, L"{CALL dbo.spLoadInventory(?)}");
    bind.BindParam(0, player_id);

    int32_t item_id = 0;
    bind.BindCol(0, item_id);
    int32_t index = 0;
    bind.BindCol(1, index);
    int32_t quantity = 0;
    bind.BindCol(2, quantity);

    if (bind.Execute()) {
      do {
        int16_t count;
        bind.GetResultColumnCount(&count);

        if (count != 3) {
          success = false;
          break;
        }

        while (bind.Fetch()) {
          if (item_id == 0 || quantity == 0) {
            continue;
          }

          const auto temp = item_id / 1000000;
          auto item_type = ItemType::kEquip;

          switch (temp) {
            case 1:
              item_type = ItemType::kEquip;
              break;
            case 2:
              item_type = ItemType::kUse;
              break;
            case 4:
              item_type = ItemType::kEtc;
              break;
            default:
              CRASH(L"Invalid item type.");
              return false;
          }

          // TODO: 아이템 추가
        }
      } while (bind.GetMoreResult() != SQL_NO_DATA);
    }

    DbConnectionPool::GetInstance().ReleaseConnection(connection);
  }

  return success;
}

bool Inventory::TrySaveToDb(int32_t player_id) {
  return true;
}
