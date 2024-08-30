#include "pch.h"
#include "map_manager.h"

#include "map_instance.h"

#include "data/map_provider.h"
#include "data/mob_provider.h"

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
  for (const auto& map : MapProvider::GetInstance().GetAllMaps() | std::views::values) {
    auto instance = std::make_shared<MapInstance>(map->GetId());

    for (const auto& spawn_point : map->GetMobs()) {
      const auto mob = MobProvider::GetInstance().GetMob(spawn_point.GetId());

      if (!mob.has_value()) {
        continue;
      }

      instance->AddSpawnLocation(std::make_shared<SpawnPoint>(spawn_point), mob.value());
    }

    _map_instances.emplace(map->GetId(), instance);
  }
}
