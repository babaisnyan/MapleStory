#include "pch.h"
#include "player.h"

#include "player_stat.h"

Player::Player(const int32_t id, const int32_t account_id, String name, const int8_t type): _id(id),
                                                                                            _account_id(account_id),
                                                                                            _name(std::move(name)),
                                                                                            _type(type),
                                                                                            _player_stat(std::make_shared<PlayerStat>()) {}

int32_t Player::GetId() const { return _id; }

int32_t Player::GetAccountId() const { return _account_id; }

const String& Player::GetName() const { return _name; }

int8_t Player::GetType() const { return _type; }

int16_t Player::GetJob() const { return _job; }

void Player::SetJob(const int16_t job) { _job = job; }

int32_t Player::GetExp() const { return _exp; }

void Player::SetExp(const int32_t exp) { _exp = exp; }

int32_t Player::GetMeso() const { return _meso; }

void Player::SetMeso(const int32_t meso) { _meso = meso; }

int32_t Player::GetMap() const { return _map; }

void Player::SetMap(const int32_t map) { _map = map; }

const GameSessionRef& Player::GetSession() const { return _session; }

void Player::SetSession(const GameSessionRef& shared) {
  _session = shared;
}

std::shared_ptr<PlayerStat> Player::GetStat() const {
  return _player_stat;
}
