#pragma once

namespace game {
  class ItemTemplate;

  class Item {
  public:
    explicit Item(const std::shared_ptr<ItemTemplate>& item_template, int32_t quantity = 1);

    const std::shared_ptr<ItemTemplate>& GetItemTemplate() const;
    uint32_t GetId() const;
    int32_t GetQuantity() const;
    void SetQuantity(int32_t quantity);

  private:
    std::shared_ptr<ItemTemplate> _item_template = nullptr;
    uint32_t _id;
    int32_t _quantity = 0;
  };
}
