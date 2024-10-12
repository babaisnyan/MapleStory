#include "pch.h"
#include "player.h"

#include "inventory.h"
#include "key_map.h"
#include "player_stat.h"

#include "database/db_bind.h"
#include "database/db_connection_pool.h"

#include "game/calc_damage.h"
#include "game/map/map_instance.h"
#include "game/map/map_manager.h"
#include "game/objects/mob/monster.h"

#include "network/protocol/game_protocol.pb.h"

Player::Player(const int32_t player_id): GameObject(GetNextObjectId()),
                                         _id(player_id),
                                         _player_stat(std::make_shared<PlayerStat>()),
                                         _key_map(std::make_shared<KeyMap>()),
                                         _inventory(std::make_shared<Inventory>()) {}

void Player::OnEnter() {
  _player_stat->UpdateStats();
}

void Player::Update(float delta_time) {}

void Player::OnCollideMob(const std::shared_ptr<Monster>& mob, const uint64_t time) {
  if (time - _player_stat->GetLastCollisionTime() < 1000) {
    return;
  }

  const auto damage = CalcDamage::GetInstance().CalcMobPhysicalDamage(mob->GetStat(), _player_stat);
  const auto map = MapManager::GetInstance().GetMapInstance(_map);

  if (map.has_value()) {
    map.value()->NotifyPlayerDamage(damage, _object_id);
  }

  OnDamage(damage, time);

  std::cout << std::format("Player is in collision area. Mob: {}, Player: {}, Damage: {}\n", mob->GetObjectId(), GetObjectId(), damage);
}

void Player::OnDamage(const int32_t damage, const uint64_t time) {
  _player_stat->SetLastCollisionTime(time);

  if (damage <= 0) {
    return;
  }

  _player_stat->SetHp(std::max(_player_stat->GetHp() - damage, 0));

  if (_player_stat->GetHp() <= 0) {
    Kill();
  }
}

void Player::Kill() {
  _is_alive = false;
  const auto map = MapManager::GetInstance().GetMapInstance(_map);

  if (map.has_value()) {
    map.value()->NotifyPlayerDeath(_object_id);
  }
}

void Player::Revive() {
  _player_stat->SetHp(_player_stat->GetMaxHp());
  _player_stat->SetMp(_player_stat->GetMaxMp());
  _is_alive = true;
}

void Player::Attack() {
  _last_attack_time = GetTickCount64();
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

bool Player::CanAttack() const {
  return _is_alive && GetTickCount64() - _last_attack_time > 700;
}

void Player::AddExp(const int32_t exp) {
  const auto need_exp = _player_stat->GetMaxExp();

  protocol::GameServerUpdatePlayerStat update;
  _player_stat->SetExp(_player_stat->GetExp() + exp);

  if (_player_stat->GetExp() >= need_exp) {
    _player_stat->SetLevel(_player_stat->GetLevel() + 1);
    _player_stat->SetExp(_player_stat->GetExp() - need_exp);
    update.set_level(_player_stat->GetLevel());
  }

  update.set_exp(_player_stat->GetExp());

  const auto map = MapManager::GetInstance().GetMapInstance(_map);

  if (map.has_value()) {
    map.value()->Send(update, _id);
  }
}

std::shared_ptr<PlayerStat> Player::GetStat() const {
  if (_player_stat->IsDirty()) {
    _player_stat->UpdateStats();
  }

  return _player_stat;
}

std::shared_ptr<KeyMap> Player::GetKeyMap() const {
  return _key_map;
}

std::shared_ptr<Inventory> Player::GetInventory() const {
  return _inventory;
}

bool Player::TryLoadFromDb() {
  bool success = true;

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

      UpdatePosition(0, 0, false);

      GetStat()->SetLevel(level);
      GetStat()->SetHp(std::max(50, hp));
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
      success = false;
    }

    DbConnectionPool::GetInstance().ReleaseConnection(connection);
  }

  if (success) {
    success &= _key_map->TryLoadFromDb(_id);
    success &= _inventory->TryLoadFromDb(_id);
  }

  return success;
}

bool Player::TrySaveToDb() {
  //TODO: 다른 정보들도 체크하기 (인벤, 스킬, 기타등등)
  if (_player_stat == nullptr || _key_map == nullptr) {
    return false;
  }

  bool success = false;

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
    int32_t x = 0;
    bind.BindParam(index++, x);
    int32_t y = 0;
    bind.BindParam(index++, y);
    int32_t ap = _player_stat->GetAp();
    bind.BindParam(index++, ap);
    int32_t sp = _player_stat->GetSp();
    bind.BindParam(index++, sp);

    int temp = 0;
    bind.BindCol(0, temp);

    if (bind.Execute() && bind.Fetch()) {
      success = temp != 0;
    }

    DbConnectionPool::GetInstance().ReleaseConnection(connection);
  }

  if (success) {
    success = _key_map->TrySaveToDb(_id);
  }

  return success;
}

int64_t Player::GetNextObjectId() {
  static std::atomic<int64_t> next_object_id = static_cast<int32_t>(kObjectRange) * static_cast<int32_t>(ObjectType::kPlayer);
  return next_object_id.fetch_add(1);
}
