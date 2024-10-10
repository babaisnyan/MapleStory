#include "pch.h"
#include "item.h"

Item::Item(const std::shared_ptr<ItemTemplate>& item_template) : _item_template(item_template) {}

const std::shared_ptr<ItemTemplate>& Item::GetItemTemplate() const {
  return _item_template;
}
