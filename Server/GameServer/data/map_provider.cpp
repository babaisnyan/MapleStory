#include "pch.h"
#include "map_provider.h"
#include "templates/map_template.h"

#include <fstream>

void MapProvider::Init() {
  std::ifstream file("Data/Maps.json");
  const auto maps = json::parse(file);

  for (const auto& map : maps) {
    auto map_template = std::make_shared<MapTemplate>();
    map_template->Load(map);
    _maps.emplace(map_template->GetId(), map_template);
  }
}
