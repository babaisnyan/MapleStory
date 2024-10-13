#include "pch.h"
#include "item.h"

#include "data/templates/item_template.h"

Item::Item(const std::shared_ptr<ItemTemplate>& item_template, const int32_t quantity) : _item_template(item_template), _id(item_template->GetId()), _quantity(quantity) {}

const std::shared_ptr<ItemTemplate>& Item::GetItemTemplate() const {
  return _item_template;
}

int32_t Item::GetId() const {
  return _id;
}

int32_t Item::GetQuantity() const {
  return _quantity;
}

void Item::SetQuantity(const int32_t quantity) {
  _quantity = quantity;
}
