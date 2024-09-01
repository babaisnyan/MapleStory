#pragma once
#include "data/ground_info.h"
#include "data/spawn_point.h"

namespace game {
  class MapTemplate {
  public:
    MapTemplate() = default;

    void Load(const json& data);

  public:
    uint32_t GetId() const;
    const String& GetName() const;
    std::pair<int32_t, int32_t> GetSize() const;
    const std::vector<SpawnPoint>& GetMobs() const;
    std::vector<GroundInfo> GetGrounds() const;

  private:
    uint32_t _id;
    String _name;
    std::pair<int32_t, int32_t> _size;
    std::vector<SpawnPoint> _mobs;
    std::vector<GroundInfo> _grounds;
  };
}
