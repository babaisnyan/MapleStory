#include "pch.h"
#include "map_provider.h"

#include <fstream>

#include "templates/map_template.h"

using namespace rapidjson;

void MapProvider::Init() {
  std::ifstream file("Data/Maps.json");
  IStreamWrapper stream(file);

  Document data;
  data.ParseStream(stream);
  assert(data.IsArray());

  for (const auto& map : data.GetArray()) {
    auto map_template = std::make_shared<MapTemplate>();
    map_template->Load(map);
    _maps.emplace(map_template->GetId(), map_template);
  }
}
