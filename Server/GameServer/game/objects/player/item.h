#pragma once

namespace game {
  class ItemTemplate;

  class Item {
  public:
    explicit Item(const std::shared_ptr<ItemTemplate>& item_template);

    const std::shared_ptr<ItemTemplate>& GetItemTemplate() const;

  private:
    std::shared_ptr<ItemTemplate> _item_template = nullptr;
  };
}
