#include "pch.h"
#include "mob_template.h"

using namespace rapidjson;

void MobTemplate::Load(const Value& data) {
  const std::string name = data["MobName"].GetString();

  _name = utils::ConvertToWide(name).value_or(L"");
  _id = data["MobId"].GetInt();
  _level = data["Level"].GetInt();
  _max_hp = data["MaxHp"].GetInt();
  _body_attack = data["BodyAttack"].GetBool();
  _first_attack = data["FirstAttack"].GetBool();
  _speed = data["Speed"].GetInt();
  _pa_damage = data["PaDamage"].GetInt();
  _ma_damage = data["MaDamage"].GetInt();
  _pd_damage = data["PdDamage"].GetInt();
  _md_damage = data["MdDamage"].GetInt();
  _pd_rate = data["PdRate"].GetInt();
  _md_rate = data["MdRate"].GetInt();
  _exp = data["Exp"].GetInt();
  _has_die = data["HasDie"].GetBool();
  _has_regen = data["HasRegen"].GetBool();
  _attack_delay = data["AttackDelay"].GetInt();
  _attack_width = data["AttackWidth"].GetInt();
  _attack_height = data["AttackHeight"].GetInt();

  if (data["HasStand"].GetBool()) {
    const auto size = std::make_pair(data["StandSize"]["X"].GetInt(), data["StandSize"]["Y"].GetInt());
    _collision_sizes.emplace(protocol::MobActionType::MOB_ACTION_TYPE_STAND, size);
  }

  if (data["HasMove"].GetBool()) {
    const auto size = std::make_pair(data["MoveSize"]["X"].GetInt(), data["MoveSize"]["Y"].GetInt());
    _collision_sizes.emplace(protocol::MobActionType::MOB_ACTION_TYPE_MOVE, size);
  }

  if (data["HasHit"].GetBool()) {
    _action_lengths.emplace(protocol::MobActionType::MOB_ACTION_TYPE_HIT, data["HitLength"].GetInt());
    const auto size = std::make_pair(data["HitSize"]["X"].GetInt(), data["HitSize"]["Y"].GetInt());
    _collision_sizes.emplace(protocol::MobActionType::MOB_ACTION_TYPE_HIT, size);
  }

  if (data["HasAttack"].GetBool()) {
    _action_lengths.emplace(protocol::MobActionType::MOB_ACTION_TYPE_ATTACK, data["AttackLength"].GetInt());
    const auto size = std::make_pair(data["AttackSize"]["X"].GetInt(), data["AttackSize"]["Y"].GetInt());
    _collision_sizes.emplace(protocol::MobActionType::MOB_ACTION_TYPE_ATTACK, size);
  }


  if (_has_regen) {
    _action_lengths.emplace(protocol::MobActionType::MOB_ACTION_TYPE_REGEN, data["RegenLength"].GetInt());
  }

  if (_has_die) {
    _action_lengths.emplace(protocol::MobActionType::MOB_ACTION_TYPE_DIE, data["DieLength"].GetInt());
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
  return true;
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

float MobTemplate::GetActionLength(const protocol::MobActionType action) const {
  return static_cast<float>(_action_lengths.at(action));
}

std::pair<int32_t, int32_t> MobTemplate::GetCollisionSize(const protocol::MobActionType action) const {
  return _collision_sizes.at(action);
}
