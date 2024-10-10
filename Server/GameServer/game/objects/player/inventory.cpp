#include "pch.h"
#include "inventory.h"

#include "item.h"

#include "data/item_provider.h"

#include "database/db_bind.h"
#include "database/db_connection_pool.h"

InventoryEntry::InventoryEntry(const std::shared_ptr<Item>& item, const int32_t count) : _item(item), _count(count) {}

void InventoryEntry::Reset() {
  _item = nullptr;
  _count = 0;
  _dirty_type = InventoryDirtyType::kNone;
}

void InventoryEntry::SetItem(const std::shared_ptr<Item>& item, const int32_t count) {
  _item = item;
  _count = count;
  _dirty_type = InventoryDirtyType::kNew;
}

std::shared_ptr<Item> InventoryEntry::GetItem() {
  return _item;
}

int32_t InventoryEntry::GetCount() const {
  return _count;
}

void InventoryEntry::UpdateCount(const int32_t count) {
  _count = count;
  _dirty_type = InventoryDirtyType::kModified;
}

InventoryEntry::InventoryDirtyType InventoryEntry::GetDirtyType() const {
  return _dirty_type;
}

void InventoryEntry::SetItemModified() {
  _dirty_type = InventoryDirtyType::kModified;
}

Inventory::Inventory() {
  std::ranges::fill(_free_slots, 0);

  for (auto& entries : _items) {
    entries.resize(kMaxSlot, std::make_shared<InventoryEntry>());
  }
}

bool Inventory::CheckFull(ItemType type) {
  if (_free_slots[static_cast<size_t>(type)] == kMaxSlot) {
    const auto pos = GetFreePos(type);
    _free_slots[static_cast<size_t>(type)] = pos;

    if (pos == kMaxSlot) {
      return true;
    }
  }

  return false;
}

bool Inventory::CanFit(ItemType type, const uint32_t id, const int16_t slot_max, int32_t count) {
  if (type == ItemType::kEquip) {
    return CheckFull(type);
  }

  for (const auto& inventory_entry : _items[static_cast<size_t>(type)]) {
    if (inventory_entry->GetItem() == nullptr) {
      count -= slot_max;
      continue;
    }

    if (inventory_entry->GetItem()->GetItemTemplate()->GetId() == id) {
      const auto current_count = inventory_entry->GetCount();

      if (current_count + count <= slot_max) {
        count -= slot_max - current_count;
      } else {
        const auto diff = slot_max - current_count - count;

        if (diff > 0) {
          count -= diff;
        }
      }

      if (count <= 0) {
        return true;
      }
    }
  }

  return false;
}

bool Inventory::AddItem(const std::shared_ptr<ItemTemplate>& item, int32_t count) {
  if (CheckFull(item->GetItemType())) {
    return false;
  }

  if (item->GetItemType() == ItemType::kEquip) {
    const auto entry = std::make_shared<Item>(item);
    return AddEquipItem(entry);
  }

  if (!CanFit(item->GetItemType(), item->GetId(), item->GetSlotMax(), count)) {
    return false;
  }

  for (const auto& inventory_entry : _items[static_cast<size_t>(item->GetItemType())]) {
    if (inventory_entry->GetItem() == nullptr) {
      const auto add_count = min(count, item->GetSlotMax());
      const auto new_item = std::make_shared<Item>(item);

      inventory_entry->SetItem(new_item, add_count);
      _all_items.push_back(inventory_entry);

      count -= add_count;
      ++_free_slots[static_cast<size_t>(item->GetItemType())];
      continue;
    }

    if (inventory_entry->GetItem()->GetItemTemplate()->GetId() == item->GetId()) {
      const auto current_count = inventory_entry->GetCount();
      const auto add_count = min(count, item->GetSlotMax() - current_count);

      inventory_entry->UpdateCount(current_count + add_count);
      count -= add_count;

      if (count <= 0) {
        return true;
      }
    }
  }

  return false;
}

bool Inventory::AddEquipItem(const std::shared_ptr<Item>& item) {
  if (CheckFull(ItemType::kEquip)) {
    return false;
  }

  const auto pos = _free_slots[static_cast<size_t>(ItemType::kEquip)];
  ++_free_slots[static_cast<size_t>(ItemType::kEquip)];

  _items[static_cast<size_t>(ItemType::kEquip)][pos]->SetItem(item);
  _all_items.push_back(_items[static_cast<size_t>(ItemType::kEquip)][pos]);

  return true;
}

const std::vector<std::shared_ptr<InventoryEntry>>& Inventory::GetAllItems() const {
  return _all_items;
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

uint8_t Inventory::GetFreePos(ItemType type) const {
  for (size_t i = 0; i < kMaxSlot; ++i) {
    if (_items[static_cast<size_t>(type)][i]->GetItem() == nullptr) {
      return i;
    }
  }

  return kMaxSlot;
}
