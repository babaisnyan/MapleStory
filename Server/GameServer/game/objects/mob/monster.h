#pragma once
#include "game/objects/game_object.h"

namespace game {
  class SpawnPoint;
  class MobTemplate;
  class MapInstance;
  class MobState;

  class Monster final : public GameObject {
  public:
    enum class State : uint8_t {
      kIdle,
      kMove,
      kAttack,
      kDead,
      kHit
    };

  public:
    Monster(const std::shared_ptr<SpawnPoint>& spawn_point, const std::shared_ptr<MapInstance>& map);
    void Init(const std::shared_ptr<MobTemplate>& mob_template);

    void OnEnter() override;
    void Update(float delta_time) override;
    void SecondUpdate(float delta_time) override;

    void UpdatePosition(float x, float y);

    uint32_t GetId() const;
    const MsCoordinate& GetPosition() const;
    float GetX() const;
    float GetY() const;

  private:
    int64_t GetNextObjectId() override;

  private:
    uint32_t _id = 0;
    MsCoordinate _position = {};
    std::shared_ptr<MobTemplate> _mob_template;
    std::shared_ptr<SpawnPoint> _spawn_point;
    std::weak_ptr<MapInstance> _map;

    std::unordered_map<State, std::shared_ptr<MobState>> _states;
    State _state = State::kIdle;
  };
}
