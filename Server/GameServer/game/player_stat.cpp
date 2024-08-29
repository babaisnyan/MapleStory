#include "pch.h"
#include "player_stat.h"

int32_t PlayerStat::GetStr() const {
  return _str;
}

void PlayerStat::SetStr(const int32_t str) {
  _str = str;
}

int32_t PlayerStat::GetDex() const {
  return _dex;
}

void PlayerStat::SetDex(const int32_t dex) {
  _dex = dex;
}

int32_t PlayerStat::GetInt() const {
  return _int;
}

void PlayerStat::SetInt(const int32_t int_) {
  _int = int_;
}

int32_t PlayerStat::GetLuk() const {
  return _luk;
}

void PlayerStat::SetLuk(const int32_t luk) {
  _luk = luk;
}

int32_t PlayerStat::GetExp() const {
  return _exp;
}

void PlayerStat::SetExp(const int32_t exp) {
  _exp = exp;
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
  _mp = mp;
}

int32_t PlayerStat::GetMaxMp() const {
  return _max_mp;
}

void PlayerStat::SetMaxMp(const int32_t max_mp) {
  _max_mp = max_mp;
}
