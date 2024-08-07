#include "pch.h"
#include "map_instance.h"

#include "game/objects/player/player.h"

#include "network/game/game_packet_creator.h"

MapInstance::MapInstance(const int32_t map_id) : _map_id(map_id) {}

bool MapInstance::AddPlayer(const std::shared_ptr<GameSession>& session) {
  const auto inserted = _players.emplace(session->GetPlayer()->GetId(), session).second;

  if (inserted) {
    _objects.emplace(session->GetPlayer()->GetObjectId(), session->GetPlayer());
  }

  return inserted;
}

bool MapInstance::RemovePlayer(const int32_t player_id) {
  const auto player = GetPlayer(player_id);

  if (!player.has_value()) {
    return false;
  }

  const auto removed = _players.erase(player_id) > 0;

  if (removed) {
    _objects.erase(player.value()->GetPlayer()->GetObjectId());

    protocol::GameServerRemovePlayer message;
    message.set_player_id(player_id);
    BroadCast(message, player_id);
  }

  return removed;
}

std::optional<std::shared_ptr<GameSession>> MapInstance::GetPlayer(const int32_t player_id) const {
  for (const auto& [id, session] : _players) {
    if (id == player_id) {
      return session;
    }
  }

  return std::nullopt;
}

std::unordered_map<int32_t, std::shared_ptr<GameSession>> MapInstance::GetPlayers() const {
  return _players;
}

int32_t MapInstance::GetMapId() const noexcept { return _map_id; }
