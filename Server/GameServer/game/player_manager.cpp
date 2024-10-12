#include "pch.h"
#include "player_manager.h"

#include "game/objects/player/player.h"

void PlayerManager::AddPlayer(const std::shared_ptr<Player>& player) {
  Accessor accessor;
  if (_players.insert(accessor, player->GetId())) {
    accessor->second = player;
  }
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