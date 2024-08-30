#pragma once

namespace game {
  class MapInstance;

  class MapManager {
  private:
    MapManager() = default;

  public:
    static MapManager& GetInstance() {
      static MapManager instance;
      return instance;
    }

  public:
    std::optional<std::shared_ptr<MapInstance>> GetMapInstance(int32_t map_id);
    std::unordered_map<int32_t, std::shared_ptr<MapInstance>> GetAllMapInstances();

    void LoadMaps();

  private:
    std::unordered_map<int32_t, std::shared_ptr<MapInstance>> _map_instances;
  };
}
