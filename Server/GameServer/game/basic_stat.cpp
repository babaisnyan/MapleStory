#include "pch.h"
#include "basic_stat.h"

bool BasicStat::IsDirty() const {
  return _is_dirty;
}

int16_t BasicStat::GetLevel() const {
  return _level;
}

void BasicStat::SetLevel(const int16_t level) {
  _is_dirty = true;
  _level = level;
}

int32_t BasicStat::GetExp() const {
  return _exp;
}

void BasicStat::SetExp(const int32_t exp) {
  _exp = exp;
}

int32_t BasicStat::GetHp() const {
  return _hp;
}

void BasicStat::SetHp(const int32_t hp) {
  _is_dirty = true;
  _hp = hp;
}

int32_t BasicStat::GetMaxHp() const {
  return _max_hp;
}

void BasicStat::SetMaxHp(const int32_t max_hp) {
  _is_dirty = true;
  _max_hp = max_hp;
}

double BasicStat::GetPhysicalAttack() const {
  return _physical_attack;
}

double BasicStat::GetPhysicalDefense() const {
  return _physical_defense;
}

double BasicStat::GetMagicalAttack() const {
  return _magical_attack;
}

double BasicStat::GetMagicalDefense() const {
  return _magical_defense;
}
