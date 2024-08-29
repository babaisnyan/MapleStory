#pragma once

namespace game {
  struct MobSpawnInfo {
    uint32_t id;
    float x;
    float y;
  };

  class MapTemplate {
  public:
    MapTemplate() = default;

    void Load(const json& data);

  public:
    uint32_t GetId() const;
    const String& GetName() const;
    const std::vector<MobSpawnInfo>& GetMobs() const;

  private:
    uint32_t _id;
    String _name;
    std::vector<MobSpawnInfo> _mobs;
  };
  
}
