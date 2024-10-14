#include "pch.h"
#include "basic_stat.h"

bool BasicStat::IsDirty() const {
  return _is_dirty;
}

void BasicStat::SetDirty() {
  _is_dirty = true;
}

int16_t BasicStat::GetLevel() const {
  return _level;
}

void BasicStat::SetLevel(const int16_t level) {
  _is_dirty = true;
  _level = min(300, level);
}

uint64_t BasicStat::GetExp() const {
  return _exp;
}

void BasicStat::SetExp(const uint64_t exp) {
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

void BasicStat::SetBasePhysicalAttack(const double base_physical_attack) {
  _base_physical_attack = base_physical_attack;
}

void BasicStat::SetBasePhysicalDefense(const double base_physical_defense) {
  _base_physical_defense = base_physical_defense;
}

void BasicStat::SetBaseMagicalAttack(const double base_magical_attack) {
  _base_magical_attack = base_magical_attack;
}

void BasicStat::SetBaseMagicalDefense(const double base_magical_defense) {
  _base_magical_defense = base_magical_defense;
}
