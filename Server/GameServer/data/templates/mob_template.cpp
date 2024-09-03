#include "pch.h"
#include "mob_template.h"

void MobTemplate::Load(const json& data) {
  std::string name;


  data.at("MobId").get_to(_id);
  data.at("MobName").get_to(name);
  _name = utils::ConvertToWide(name).value_or(L"");
  data.at("Level").get_to(_level);
  data.at("MaxHp").get_to(_max_hp);
  data.at("BodyAttack").get_to(_body_attack);
  data.at("FirstAttack").get_to(_first_attack);
  data.at("Speed").get_to(_speed);
  data.at("PaDamage").get_to(_pa_damage);
  data.at("MaDamage").get_to(_ma_damage);
  data.at("PdDamage").get_to(_pd_damage);
  data.at("MdDamage").get_to(_md_damage);
  data.at("PdRate").get_to(_pd_rate);
  data.at("MdRate").get_to(_md_rate);
  data.at("Exp").get_to(_exp);
  data.at("HasDie").get_to(_has_die);
  data.at("HasRegen").get_to(_has_regen);
  data.at("AttackDelay").get_to(_attack_delay);
  data.at("AttackWidth").get_to(_attack_width);
  data.at("AttackHeight").get_to(_attack_height);

  if (data.at("HasStand").get<bool>()) {
    const auto size = std::make_pair(data["StandSize"]["X"].get<int32_t>(), data["StandSize"]["Y"].get<int32_t>());
    _collision_sizes.emplace(protocol::MobActionType::MOB_ACTION_TYPE_STAND, size);
  }

  if (data.at("HasMove").get<bool>()) {
    const auto size = std::make_pair(data["MoveSize"]["X"].get<int32_t>(), data["MoveSize"]["Y"].get<int32_t>());
    _collision_sizes.emplace(protocol::MobActionType::MOB_ACTION_TYPE_MOVE, size);
  }

  if (data.at("HasHit").get<bool>()) {
    _action_lengths.emplace(protocol::MobActionType::MOB_ACTION_TYPE_HIT, data.at("HitLength").get<int32_t>());
    const auto size = std::make_pair(data["HitSize"]["X"].get<int32_t>(), data["HitSize"]["Y"].get<int32_t>());
    _collision_sizes.emplace(protocol::MobActionType::MOB_ACTION_TYPE_HIT, size);
  }

  if (data.at("HasAttack").get<bool>()) {
    _action_lengths.emplace(protocol::MobActionType::MOB_ACTION_TYPE_ATTACK, data.at("AttackLength").get<int32_t>());
    const auto size = std::make_pair(data["AttackSize"]["X"].get<int32_t>(), data["AttackSize"]["Y"].get<int32_t>());
    _collision_sizes.emplace(protocol::MobActionType::MOB_ACTION_TYPE_ATTACK, size);
  }

  if (_has_regen) {
    _action_lengths.emplace(protocol::MobActionType::MOB_ACTION_TYPE_REGEN, data.at("RegenLength").get<int32_t>());
  }

  if (_has_die) {
    _action_lengths.emplace(protocol::MobActionType::MOB_ACTION_TYPE_DIE, data.at("DieLength").get<int32_t>());
  }
}

int16_t MobTemplate::GetLevel() const {
  return _level;
}

int32_t MobTemplate::GetMaxHp() const {
  return _max_hp;
}

bool MobTemplate::CanBodyAttack() const {
  return _body_attack;
}

bool MobTemplate::CanFirstAttack() const {
  return _first_attack;
}

int16_t MobTemplate::GetSpeed() const {
  return _speed;
}

int16_t MobTemplate::GetPaDamage() const {
  return _pa_damage;
}

int16_t MobTemplate::GetMaDamage() const {
  return _ma_damage;
}

int16_t MobTemplate::GetPdDamage() const {
  return _pd_damage;
}

int16_t MobTemplate::GetMdDamage() const {
  return _md_damage;
}

int16_t MobTemplate::GetPdRate() const {
  return _pd_rate;
}

int16_t MobTemplate::GetMdRate() const {
  return _md_rate;
}

int16_t MobTemplate::GetExp() const {
  return _exp;
}

bool MobTemplate::HasAction(const protocol::MobActionType action) const {
  if (action == protocol::MobActionType::MOB_ACTION_TYPE_DIE) {
    return _has_die;
  }

  if (action == protocol::MobActionType::MOB_ACTION_TYPE_REGEN) {
    return _has_regen;
  }

  return _collision_sizes.contains(action);
}

int16_t MobTemplate::GetAttackDelay() const {
  return _attack_delay;
}

int16_t MobTemplate::GetAttackWidth() const {
  return _attack_width;
}

int16_t MobTemplate::GetAttackHeight() const {
  return _attack_height;
}

int32_t MobTemplate::GetActionLength(const protocol::MobActionType action) const {
  return _action_lengths.at(action);
}

std::pair<int32_t, int32_t> MobTemplate::GetCollisionSize(protocol::MobActionType action) const {
  return _collision_sizes.at(action);
}
