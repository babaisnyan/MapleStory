#include "pch.h"
#include "player_stat.h"

int32_t PlayerStat::GetStr() const { return _str; }

void PlayerStat::SetStr(const int32_t str) { _str = str; }

int32_t PlayerStat::GetDex() const { return _dex; }

void PlayerStat::SetDex(const int32_t dex) { _dex = dex; }

int32_t PlayerStat::GetInt() const { return _int; }

void PlayerStat::SetInt(const int32_t int_) { _int = int_; }

int32_t PlayerStat::GetLuk() const { return _luk; }

void PlayerStat::SetLuk(const int32_t luk) { _luk = luk; }
