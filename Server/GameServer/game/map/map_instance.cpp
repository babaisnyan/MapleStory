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

bool MapInstance::RemoveObject(const int64_t object_id) {
  const auto type = static_cast<GameObject::ObjectType>(object_id / GameObject::kObjectRange);
  int32_t player_id = -1;

  if (type == GameObject::ObjectType::kPlayer) {
    const auto player = GetPlayer(object_id);

    if (!player.has_value()) {
      return false;
    }

    player_id = player.value()->GetPlayer()->GetId();

    if (_players.erase(player_id) <= 0) {
      return false;
    }
  }

  const auto removed = _objects.erase(object_id) > 0;

  protocol::GameServerRemoveObject message;
  message.set_object_id(object_id);

  BroadCast(message, player_id);

  return removed;
}

std::optional<std::shared_ptr<GameSession>> MapInstance::GetPlayer(const int64_t object_id) const {
  const auto type = static_cast<GameObject::ObjectType>(object_id / GameObject::kObjectRange);

  if (type != GameObject::ObjectType::kPlayer) {
    return std::nullopt;
  }

  if (!_objects.contains(object_id)) {
    return std::nullopt;
  }

  const auto player = std::static_pointer_cast<Player>(_objects.at(object_id));
  const auto session = GetPlayer(player->GetId());
  return session;
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
