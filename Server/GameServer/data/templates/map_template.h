#pragma once
#include "data/spawn_point.h"

namespace game {
  class MapTemplate {
  public:
    MapTemplate() = default;

    void Load(const json& data);

  public:
    uint32_t GetId() const;
    const String& GetName() const;
    const std::vector<SpawnPoint>& GetMobs() const;

  private:
    uint32_t _id;
    String _name;
    std::vector<SpawnPoint> _mobs;
  };
}
