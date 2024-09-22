#include "pch.h"
#include "item_provider.h"

#include <fstream>

#include "templates/item_template.h"

using namespace rapidjson;

void ItemProvider::Init() {
  std::ifstream file("Data/Items.json");
  IStreamWrapper stream(file);

  Document data;
  data.ParseStream(stream);
  assert(data.IsArray());

  for (const auto& item : data.GetArray()) {
    auto item_template = std::make_shared<ItemTemplate>();
    item_template->Load(item);
    _items.emplace(item_template->GetId(), item_template);
  }
}
