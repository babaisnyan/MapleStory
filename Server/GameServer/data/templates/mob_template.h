#pragma once

namespace game {
  class MobTemplate {
  public:
    MobTemplate() = default;

    void Load(const json& data);

  public:
    uint32_t GetId() const;
    const String& GetName() const;
    int16_t GetLevel() const;
    int32_t GetMaxHp() const;
    bool GetBodyAttack() const;
    bool GetFirstAttack() const;
    int16_t GetSpeed() const;
    int16_t GetPaDamage() const;
    int16_t GetMaDamage() const;
    int16_t GetPdDamage() const;
    int16_t GetMdDamage() const;
    int16_t GetPdRate() const;
    int16_t GetMdRate() const;
    int16_t GetExp() const;
    bool GetHasStand() const;
    bool GetHasMove() const;
    bool GetHasAttack() const;
    int16_t GetAttackCool() const;
    int16_t GetAttackWidth() const;
    int16_t GetAttackHeight() const;

  private:
    uint32_t _id;
    String _name;
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
    bool _has_stand;
    bool _has_move;
    bool _has_attack;
    int16_t _attack_cool;
    int16_t _attack_width;
    int16_t _attack_height;
  };
  
}
