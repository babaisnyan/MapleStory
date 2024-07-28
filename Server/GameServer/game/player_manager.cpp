#include "pch.h"
#include "player_manager.h"

#include "player.h"

#include "database/db_bind.h"
#include "database/db_connection_pool.h"

bool PlayerManager::AddPlayer(const int32_t player_id, const GameSessionRef& session) {
  auto player = LoadPlayer(player_id);

  if (!player.has_value()) {
    return false;
  }

  player.value()->SetSession(session);

  {
    Accessor accessor;
    if (_players.insert(accessor, player.value()->GetId())) {
      accessor->second = player.value();
      return true;
    }
  }

  return false;
}

void PlayerManager::RemovePlayer(const int32_t player_id) {
  Accessor accessor;

  if (_players.find(accessor, player_id)) {
    _players.erase(accessor);
  }
}

std::optional<std::shared_ptr<Player>> PlayerManager::Find(const int32_t player_id) const {
  ConstAccessor accessor;

  if (_players.find(accessor, player_id)) {
    return accessor->second;
  }

  return std::nullopt;
}

std::optional<std::shared_ptr<Player>> PlayerManager::LoadPlayer(int32_t player_id) const {
  if (const auto connection = DbConnectionPool::GetInstance().GetConnection()) {
    std::shared_ptr<Player> player;
    DbBind<1, 17> bind(*connection, L"{CALL dbo.spLoadCharacter(?)}");
    bind.BindParam(0, player_id);

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

    if (bind.Execute()) {
      player = std::make_shared<Player>(id, account_id, name, type, level, exp, meso, map, hp, mp, max_hp, max_mp, str, dex, int_, luk);
    }

    return player;

    DbConnectionPool::GetInstance().ReleaseConnection(connection);
  }

  return std::nullopt;
}
