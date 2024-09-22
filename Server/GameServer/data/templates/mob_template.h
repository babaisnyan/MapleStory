#pragma once
#include "template.h"

#include "network/protocol/game_enum.pb.h"

namespace game {
  class MobTemplate : public Template {
  public:
    MobTemplate() = default;

    void Load(const rapidjson::Value& data) override;

  public:
    int16_t GetLevel() const;
    int32_t GetMaxHp() const;
    bool CanBodyAttack() const;
    bool CanFirstAttack() const;
    int16_t GetSpeed() const;
    int16_t GetPaDamage() const;
    int16_t GetMaDamage() const;
    int16_t GetPdDamage() const;
    int16_t GetMdDamage() const;
    int16_t GetPdRate() const;
    int16_t GetMdRate() const;
    int16_t GetExp() const;
    bool HasAction(protocol::MobActionType action) const;
    int16_t GetAttackDelay() const;
    int16_t GetAttackWidth() const;
    int16_t GetAttackHeight() const;
    int32_t GetActionLength(protocol::MobActionType action) const;
    std::pair<int32_t, int32_t> GetCollisionSize(protocol::MobActionType action) const;

  private:
    int16_t _level;
    int32_t _max_hp;
    bool _body_attack;
    bool _first_attack;
    int16_t _speed;
    int16_t _pa_damage;
    int16_t _ma_damage;
    int16_t _pd_damage;
    int16_t _md_damage;
    int16_t _pd_rate;
    int16_t _md_rate;
    int16_t _exp;
    bool _has_die;
    bool _has_regen;
    int16_t _attack_delay;
    int16_t _attack_width;
    int16_t _attack_height;
    std::unordered_map<protocol::MobActionType, int32_t> _action_lengths;
    std::unordered_map<protocol::MobActionType, std::pair<int32_t, int32_t>> _collision_sizes;
  };
  
}
