#pragma once
#include "template.h"

#include "data/ground_info.h"
#include "data/spawn_point.h"

namespace game {
  class MapTemplate : public Template {
  public:
    MapTemplate() = default;

    void Load(const json& data) override;

  public:
    std::pair<int32_t, int32_t> GetSize() const;
    const std::vector<SpawnPoint>& GetMobs() const;
    std::vector<GroundInfo> GetGrounds() const;

  private:
    std::pair<int32_t, int32_t> _size;
    std::vector<SpawnPoint> _mobs;
    std::vector<GroundInfo> _grounds;
  };
}
