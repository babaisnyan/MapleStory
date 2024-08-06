#include "pch.h"
#include "map_manager.h"

#include "map_instance.h"

MapManager::MapManager() {
  LoadMaps();
}

std::optional<std::shared_ptr<MapInstance>> MapManager::GetMapInstance(const int32_t map_id) {
  const auto it = _map_instances.find(map_id);

  if (it != _map_instances.end()) {
    return it->second;
  }

  return std::nullopt;
}

std::unordered_map<int32_t, std::shared_ptr<MapInstance>> MapManager::GetAllMapInstances() {
  return _map_instances;
}

void MapManager::LoadMaps() {
  _map_instances.emplace(0, std::make_shared<MapInstance>(0));
}
