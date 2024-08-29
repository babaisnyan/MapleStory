#include "pch.h"
#include "basic_stat.h"

int16_t BasicStat::GetLevel() const { return _level; }

void BasicStat::SetLevel(const int16_t level) { _level = level; }

int32_t BasicStat::GetHp() const { return _hp; }

void BasicStat::SetHp(const int32_t hp) { _hp = hp; }

int32_t BasicStat::GetMaxHp() const { return _max_hp; }

void BasicStat::SetMaxHp(const int32_t max_hp) { _max_hp = max_hp; }
