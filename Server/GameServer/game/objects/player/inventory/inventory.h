#pragma once

#include <shared_mutex>

#include "data/templates/item_template.h"

namespace game {
  class Item;

  class Inventory {
  public:
    enum : uint8_t {
      kMaxSlot = 128,
      kMinEquipSlot = 1,
      kMaxEquipSlot = 13
    };

    enum InventoryType : uint8_t {
      kEquip    = 0,
      kUse      = 1,
      kEtc      = 2,
      kEquipped = 3,
      kEnd
    };

  public:
    Inventory();

    bool AddItem(InventoryType type, int32_t pos, const std::shared_ptr<Item>& item);
    bool AddItem(InventoryType type, const std::shared_ptr<ItemTemplate>& item_template, int32_t quantity = 1);
    [[nodiscard]] std::optional<std::shared_ptr<Item>> GetItem(InventoryType type, int32_t pos) const;
    void SwapItem(InventoryType type, int32_t src, int32_t dst);
    bool RemoveItem(InventoryType type, int32_t pos);
    bool RemoveItem(InventoryType type, int32_t pos, int32_t quantity);
    [[nodiscard]] std::map<int32_t, std::shared_ptr<Item>> GetAllItems(InventoryType type) const;
    bool Equip(int32_t src, int32_t dst);
    bool Unequip(int32_t src, int32_t dst);

    bool CanHold(InventoryType type, uint32_t id, int32_t quantity, int32_t max_count) const;

  public:
    /*PlayerDbQueue 에서 호출*/
    bool TryLoadFromDb(int32_t player_id);
    bool TrySaveToDb(int32_t player_id);

  private:
    std::wstring SerializeToXml();

  private:
    std::array<std::map<int32_t, std::shared_ptr<Item>>, kEnd> _items; // 0: Equip, 1: Use, 2: Etc, 3: Equipped
    std::vector<std::pair<uint8_t, int32_t>> _removed_items;
    mutable std::array<std::shared_mutex, kEnd> _items_mutex;
    mutable std::shared_mutex _removed_items_mutex;
  };
}
