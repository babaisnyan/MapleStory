#include "pch.h"
#include "player.h"

#include "database/db_bind.h"
#include "database/db_connection_pool.h"
#include "game/player_stat.h"
#include "game/map/map_instance.h"
#include "game/map/map_manager.h"


Player::Player(const int32_t id): _id(id), _player_stat(std::make_shared<PlayerStat>()) {}

void Player::OnEnter() {
  // TODO: 기타 정보 보내주기

}

void Player::Update(float delta_time) {}

void Player::UpdatePosition(const int16_t x, const int16_t y) {
  _position.x = x;
  _position.y = y;
  _position.grid_x = x / _position.grid_size;
  _position.grid_y = y / _position.grid_size;
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

    int32_t id;
    bind.BindCol(0, id);
    int32_t account_id;
    bind.BindCol(1, account_id);
    wchar_t name[50];
    bind.BindCol(2, name);
    int8_t type;
    bind.BindCol(3, type);
    int16_t level;
    bind.BindCol(4, level);
    int16_t job;
    bind.BindCol(5, job);
    int32_t exp;
    bind.BindCol(6, exp);
    int32_t meso;
    bind.BindCol(7, meso);
    int32_t map;
    bind.BindCol(8, map);
    int32_t hp;
    bind.BindCol(9, hp);
    int32_t mp;
    bind.BindCol(10, mp);
    int32_t max_hp;
    bind.BindCol(11, max_hp);
    int32_t max_mp;
    bind.BindCol(12, max_mp);
    int16_t str;
    bind.BindCol(13, str);
    int16_t dex;
    bind.BindCol(14, dex);
    int16_t luk;
    bind.BindCol(15, luk);
    int16_t int_;
    bind.BindCol(16, int_);
    int16_t x;
    bind.BindCol(17, x);
    int16_t y;
    bind.BindCol(18, y);
    int16_t ap;
    bind.BindCol(19, ap);
    int16_t sp;
    bind.BindCol(20, sp);

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
  return false;
}
