#include "pch.h"
#include "mob_template.h"

void MobTemplate::Load(const json& data) {
  std::string name;

  data.at("Id").get_to(_id);
  data.at("Name").get_to(name);
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
  data.at("HasStand").get_to(_has_stand);
  data.at("HasMove").get_to(_has_move);
  data.at("HasAttack").get_to(_has_attack);
  data.at("AttackCool").get_to(_attack_cool);
  data.at("AttackWidth").get_to(_attack_width);
  data.at("AttackHeight").get_to(_attack_height);

}

uint32_t MobTemplate::GetId() const {
  return _id;
}

const String& MobTemplate::GetName() const {
  return _name;
}

int16_t MobTemplate::GetLevel() const {
  return _level;
}

int32_t MobTemplate::GetMaxHp() const {
  return _max_hp;
}

bool MobTemplate::GetBodyAttack() const {
  return _body_attack;
}

bool MobTemplate::GetFirstAttack() const {
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

bool MobTemplate::GetHasStand() const {
  return _has_stand;
}

bool MobTemplate::GetHasMove() const {
  return _has_move;
}

bool MobTemplate::GetHasAttack() const {
  return _has_attack;
}

int16_t MobTemplate::GetAttackCool() const {
  return _attack_cool;
}

int16_t MobTemplate::GetAttackWidth() const {
  return _attack_width;
}

int16_t MobTemplate::GetAttackHeight() const {
  return _attack_height;
}
