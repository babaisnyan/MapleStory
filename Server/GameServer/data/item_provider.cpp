#include "pch.h"
#include "item_provider.h"

#include "templates/item_template.h"

#include <fstream>


void ItemProvider::Init() {
  std::ifstream file("Data/Items.json");
  const auto items = json::parse(file);

  for (const auto& item : items) {
    auto item_template = std::make_shared<ItemTemplate>();
    item_template->Load(item);
    _items.emplace(item_template->GetId(), item_template);
  }
}
