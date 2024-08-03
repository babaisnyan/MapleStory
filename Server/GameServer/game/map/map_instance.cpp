#include "pch.h"
#include "map_instance.h"

#include "game/objects/player/player.h"

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

    //TODO: 패킷 전송
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

int32_t MapInstance::GetMapId() const noexcept { return _map_id; }
