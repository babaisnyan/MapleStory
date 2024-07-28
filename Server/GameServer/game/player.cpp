#include "pch.h"
#include "player.h"

int32_t Player::GetId() const { return _id; }

int32_t Player::GetAccountId() const { return _account_id; }

const String& Player::GetName() const { return _name; }

int8_t Player::GetType() const { return _type; }

int16_t Player::GetLevel() const { return _level; }

int16_t Player::GetJob() const { return _job; }

int32_t Player::GetExp() const { return _exp; }

int32_t Player::GetMeso() const { return _meso; }

int32_t Player::GetMap() const { return _map; }

int32_t Player::GetHp() const { return _hp; }

int32_t Player::GetMp() const { return _mp; }

int32_t Player::GetMaxHp() const { return _max_hp; }

int32_t Player::GetMaxMp() const { return _max_mp; }

int32_t Player::GetStr() const { return _str; }

int32_t Player::GetDex() const { return _dex; }

int32_t Player::GetInt() const { return _int; }

int32_t Player::GetLuk() const { return _luk; }

void Player::SetSession(const GameSessionRef& shared) {
  _session = shared;
}
