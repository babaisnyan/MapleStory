#include "pch.h"
#include "player_stat.h"

void PlayerStat::UpdateStats() {
  _base_physical_defense = _level * 2.5;
  _base_magical_defense = _level * 2.5;

  // TODO: 장비, 버프에 의한 스탯 증가 적용

  _stat_base = (_str + _dex + _int + _luk) * 0.25;

  _is_dirty = false;
}

int32_t PlayerStat::GetStr() const {
  return _str;
}

void PlayerStat::SetStr(const int32_t str) {
  _is_dirty = true;
  _str = str;
}

int32_t PlayerStat::GetDex() const {
  return _dex;
}

void PlayerStat::SetDex(const int32_t dex) {
  _is_dirty = true;
  _dex = dex;
}

int32_t PlayerStat::GetInt() const {
  return _int;
}

void PlayerStat::SetInt(const int32_t int_) {
  _is_dirty = true;
  _int = int_;
}

int32_t PlayerStat::GetLuk() const {
  return _luk;
}

void PlayerStat::SetLuk(const int32_t luk) {
  _is_dirty = true;
  _luk = luk;
}

int32_t PlayerStat::GetAp() const {
  return _ap;
}

void PlayerStat::SetAp(const int32_t ap) {
  _ap = ap;
}

int32_t PlayerStat::GetSp() const {
  return _sp;
}

void PlayerStat::SetSp(const int32_t sp) {
  _sp = sp;
}

int32_t PlayerStat::GetMp() const {
  return _mp;
}

void PlayerStat::SetMp(const int32_t mp) {
  _is_dirty = true;
  _mp = mp;
}

int32_t PlayerStat::GetMaxMp() const {
  return _max_mp;
}

void PlayerStat::SetMaxMp(const int32_t max_mp) {
  _is_dirty = true;
  _max_mp = max_mp;
}

uint64_t PlayerStat::GetLastCollisionTime() const {
  return _last_collision_time;
}

void PlayerStat::SetLastCollisionTime(const uint64_t last_collision_time) {
  _last_collision_time = last_collision_time;
}

double PlayerStat::GetStatBase() const {
  return _stat_base;
}
