#include "pch.h"
#include "basic_stat.h"

int16_t BasicStat::GetLevel() const { return _level; }

void BasicStat::SetLevel(const int16_t level) { _level = level; }

int32_t BasicStat::GetHp() const { return _hp; }

void BasicStat::SetHp(const int32_t hp) { _hp = hp; }

int32_t BasicStat::GetMp() const { return _mp; }

void BasicStat::SetMp(const int32_t mp) { _mp = mp; }

int32_t BasicStat::GetMaxHp() const { return _max_hp; }

void BasicStat::SetMaxHp(const int32_t max_hp) { _max_hp = max_hp; }

int32_t BasicStat::GetMaxMp() const { return _max_mp; }

void BasicStat::SetMaxMp(const int32_t max_mp) { _max_mp = max_mp; }
