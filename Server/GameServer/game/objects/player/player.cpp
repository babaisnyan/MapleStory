﻿#include "pch.h"
#include "player.h"

#include "database/db_bind.h"
#include "database/db_connection_pool.h"
#include "game/player_stat.h"


Player::Player(const int32_t player_id): GameObject(GetNextObjectId()), _id(player_id), _player_stat(std::make_shared<PlayerStat>()) {}

void Player::OnEnter() {
  // TODO: 기타 정보 보내주기
}

void Player::Update(float delta_time) {}

void Player::UpdatePosition(const float x, const float y) {
  _position.x = x;
  _position.y = y;
  _position.grid_x = static_cast<int32_t>(x) / _position.grid_size;
  _position.grid_y = static_cast<int32_t>(y) / _position.grid_size;
}

int32_t Player::GetId() const {
  return _id;
}

int32_t Player::GetAccountId() const {
  return _account_id;
}

const String& Player::GetName() const {
  return _name;
}

int8_t Player::GetType() const {
  return _type;
}

int16_t Player::GetJob() const {
  return _job;
}

void Player::SetJob(const int16_t job) {
  _job = job;
}

int32_t Player::GetMeso() const {
  return _meso;
}

void Player::SetMeso(const int32_t meso) {
  _meso = meso;
}

int32_t Player::GetMap() const {
  return _map;
}

void Player::SetMap(const int32_t map) {
  _map = map;
}

const MsCoordinate& Player::GetPosition() const {
  return _position;
}

std::shared_ptr<PlayerStat> Player::GetStat() const {
  return _player_stat;
}

bool Player::TryLoadFromDb() {
  if (const auto connection = DbConnectionPool::GetInstance().GetConnection()) {
    DbBind<1, 21> bind(*connection, L"{CALL dbo.spLoadCharacter(?)}");
    bind.BindParam(0, _id);

    int32_t index = 0;
    int32_t id;
    bind.BindCol(index++, id);
    int32_t account_id;
    bind.BindCol(index++, account_id);
    wchar_t name[50];
    bind.BindCol(index++, name);
    int8_t type;
    bind.BindCol(index++, type);
    int16_t level;
    bind.BindCol(index++, level);
    int16_t job;
    bind.BindCol(index++, job);
    int32_t exp;
    bind.BindCol(index++, exp);
    int32_t meso;
    bind.BindCol(index++, meso);
    int32_t map;
    bind.BindCol(index++, map);
    int32_t hp;
    bind.BindCol(index++, hp);
    int32_t mp;
    bind.BindCol(index++, mp);
    int32_t max_hp;
    bind.BindCol(index++, max_hp);
    int32_t max_mp;
    bind.BindCol(index++, max_mp);
    int16_t str;
    bind.BindCol(index++, str);
    int16_t dex;
    bind.BindCol(index++, dex);
    int16_t luk;
    bind.BindCol(index++, luk);
    int16_t int_;
    bind.BindCol(index++, int_);
    int16_t x;
    bind.BindCol(index++, x);
    int16_t y;
    bind.BindCol(index++, y);
    int16_t ap;
    bind.BindCol(index++, ap);
    int16_t sp;
    bind.BindCol(index++, sp);

    if (bind.Execute() && bind.Fetch()) {
      _id = id;
      _account_id = account_id;
      _name = name;
      _type = type;
      _job = job;
      _meso = meso;
      _map = map;

      UpdatePosition(x, y);

      GetStat()->SetLevel(level);
      GetStat()->SetHp(hp);
      GetStat()->SetMp(mp);
      GetStat()->SetMaxHp(max_hp);
      GetStat()->SetMaxMp(max_mp);
      GetStat()->SetStr(str);
      GetStat()->SetDex(dex);
      GetStat()->SetLuk(luk);
      GetStat()->SetInt(int_);
      GetStat()->SetExp(exp);
      GetStat()->SetAp(ap);
      GetStat()->SetSp(sp);
    } else {
      return false;
    }

    DbConnectionPool::GetInstance().ReleaseConnection(connection);
    return true;
  }

  return false;
}

bool Player::TrySaveToDb() {
  //TODO: 다른 정보들도 체크하기 (인벤, 스킬, 기타등등)
  if (_player_stat == nullptr) {
    return false;
  }


  if (const auto connection = DbConnectionPool::GetInstance().GetConnection()) {
    DbBind<18, 1> bind(*connection, L"{CALL dbo.spSaveCharacter(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}");
    int32_t index = 0;

    bind.BindParam(index++, _id);
    int16_t level = _player_stat->GetLevel();
    bind.BindParam(index++, level);
    bind.BindParam(index++, _job);
    int32_t exp = _player_stat->GetExp();
    bind.BindParam(index++, exp);
    bind.BindParam(index++, _meso);
    bind.BindParam(index++, _map);
    int32_t hp = _player_stat->GetHp();
    bind.BindParam(index++, hp);
    int32_t mp = _player_stat->GetMp();
    bind.BindParam(index++, mp);
    int32_t max_hp = _player_stat->GetMaxHp();
    bind.BindParam(index++, max_hp);
    int32_t max_mp = _player_stat->GetMaxMp();
    bind.BindParam(index++, max_mp);
    int32_t str = _player_stat->GetStr();
    bind.BindParam(index++, str);
    int32_t dex = _player_stat->GetDex();
    bind.BindParam(index++, dex);
    int32_t luk = _player_stat->GetLuk();
    bind.BindParam(index++, luk);
    int32_t int_ = _player_stat->GetInt();
    bind.BindParam(index++, int_);
    int32_t x = static_cast<int32_t>(_position.x);
    bind.BindParam(index++, x);
    int32_t y = static_cast<int32_t>(_position.y);
    bind.BindParam(index++, y);
    int32_t ap = _player_stat->GetAp();
    bind.BindParam(index++, ap);
    int32_t sp = _player_stat->GetSp();
    bind.BindParam(index++, sp);

    int success = 0;
    bind.BindCol(0, success);

    if (bind.Execute() && bind.Fetch()) {
      DbConnectionPool::GetInstance().ReleaseConnection(connection);
      return success != 0;
    }

    DbConnectionPool::GetInstance().ReleaseConnection(connection);
  }

  return false;
}

int64_t Player::GetNextObjectId() {
  static std::atomic<int64_t> next_object_id = static_cast<int32_t>(kObjectRange) * static_cast<int32_t>(ObjectType::kPlayer);
  return next_object_id.fetch_add(1);
}