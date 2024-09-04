﻿#pragma once
#include "data/templates/mob_template.h"

#include "game/objects/game_object.h"

namespace game {
  class Player;
  class SpawnPoint;
  class MobTemplate;
  class MapInstance;
  class MobState;

  class Monster final : public GameObject {
  public:
    Monster(const std::shared_ptr<SpawnPoint>& spawn_point, const std::shared_ptr<MapInstance>& map);
    void Init(const std::shared_ptr<MobTemplate>& mob_template);

    void OnEnter() override;
    void Update(float delta_time) override;
    void PostUpdate() override;

    std::shared_ptr<Monster> GetSelf();
    uint32_t GetId() const;
    std::shared_ptr<MobTemplate> GetTemplate() const;
    std::weak_ptr<MapInstance> GetMap() const;
    std::weak_ptr<Player> GetTarget() const;
    float GetSpeed() const;

    float GetAnimationTime() const;
    void AddAnimationTime(float delta_time);
    void ResetAnimationTime();

    void ResetTarget();
    void ChangeTarget(const std::shared_ptr<Player>& player);
    bool IsTargetInDistance() const;
    bool HasTarget() const;

    bool IsTargetInAttackRange() const;
    bool IsAttackReady() const;

    bool IsCollisionEnabled() const;
    void SetCollisionEnabled(bool enabled);
    uint64_t GetRegenEndTime() const;
    void SetRegenEndTime(uint64_t regen_end_time);

  public:
    void ChangeState(protocol::MobActionType state);

  private:
    int64_t GetNextObjectId() override;

  private:
    static void InitMonsterStates();

  private:
    uint32_t _id = 0;
    std::shared_ptr<MobTemplate> _mob_template;
    std::shared_ptr<SpawnPoint> _spawn_point;
    std::weak_ptr<MapInstance> _map;
    std::weak_ptr<Player> _target;

    float _speed = 2.0f;

  private:
    std::unordered_map<protocol::MobActionType, std::shared_ptr<MobState>> _states;
    protocol::MobActionType _current_state = protocol::MobActionType::MOB_ACTION_TYPE_UNSPECIFIED;

    float _animation_time = 0.0f;
    bool _is_collision_enabled = true;
    uint64_t _regen_end_time = 0;
    uint64_t _next_attack_time = 0;

  private:
    inline static std::once_flag _init_flag;
    inline static std::unordered_map<protocol::MobActionType, std::shared_ptr<MobState>> _monster_states;
  };
}
