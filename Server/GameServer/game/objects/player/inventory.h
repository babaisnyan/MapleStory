#pragma once
#include "data/templates/item_template.h"

namespace game {
  class Item;

  class InventoryEntry {
  public:
    enum class InventoryDirtyType : uint8_t {
      kNone,
      kNew,
      kModified
    };

  public:
    InventoryEntry() = default;
    explicit InventoryEntry(const std::shared_ptr<Item>& item, int32_t count = 1);

    void Reset();

    void SetItem(const std::shared_ptr<Item>& item, int32_t count = 1);
    std::shared_ptr<Item> GetItem();

    int32_t GetCount() const;
    void UpdateCount(int32_t count);

    InventoryDirtyType GetDirtyType() const;
    void SetItemModified();

  private:
    std::shared_ptr<Item> _item = nullptr;
    int32_t _count = 0;
    InventoryDirtyType _dirty_type = InventoryDirtyType::kNone;
  };

  class Inventory {
    enum : uint8_t {
      kMaxSlot = 128
    };

  public:
    Inventory();

    bool CheckFull(ItemType type);
    bool CanFit(ItemType type, uint32_t id, int16_t slot_max, int32_t count = 1);
    bool AddItem(const std::shared_ptr<ItemTemplate>& item, int32_t count = 1);
    bool AddEquipItem(const std::shared_ptr<Item>& item);

  public:
    const std::vector<std::shared_ptr<InventoryEntry>>& GetAllItems() const;

  public:
    /*PlayerDbQueue 에서 호출*/
    bool TryLoadFromDb(int32_t player_id);
    bool TrySaveToDb(int32_t player_id);

  private:
    uint8_t GetFreePos(ItemType type) const;

  private:
    std::array<uint8_t, static_cast<size_t>(ItemType::kEtc) + 1> _free_slots;
    std::array<std::vector<std::shared_ptr<InventoryEntry>>, static_cast<size_t>(ItemType::kEtc) + 1> _items;
    std::map<size_t, std::shared_ptr<InventoryEntry>> _equipped_items;
    std::vector<std::shared_ptr<Item>> _removed_items;
    std::vector<std::shared_ptr<InventoryEntry>> _all_items;
  };
}
