#include "pch.h"
#include "map_instance.h"

#include "game/player_stat.h"
#include "game/objects/player/player.h"

MapInstance::MapInstance(const int32_t map_id) : _map_id(map_id) {}

bool MapInstance::AddPlayer(const std::shared_ptr<GameSession>& session) {
  const auto inserted = _players.emplace(session->GetPlayer()->GetId(), session).second;

  if (inserted) {
    _objects.emplace(session->GetPlayer()->GetObjectId(), session->GetPlayer());
    OnPlayerEnter(session);
  }

  return inserted;
}

bool MapInstance::RemoveObject(const int64_t object_id) {
  const auto type = static_cast<GameObject::ObjectType>(object_id / GameObject::kObjectRange);

  if (type == GameObject::ObjectType::kPlayer) {
    const auto player_session = GetPlayer(object_id);

    if (!player_session.has_value()) {
      return false;
    }

    const auto player = player_session.value()->GetPlayer();
    const auto player_id = player->GetId();

    if (_players.erase(player_id) <= 0) {
      return false;
    }

    OnPlayerLeave(player);
  }

  if (_objects.erase(object_id) <= 0) {
    return false;
  }

  return true;
}

void MapInstance::MovePlayer(const std::shared_ptr<GameSession>& session, const protocol::GameClientPlayerMove& packet) {
  const auto player = session->GetPlayer();

  if (!player) {
    return;
  }

  player->UpdatePosition(packet.x(), packet.y());

  protocol::GameServerPlayerMove response;
  response.set_object_id(player->GetObjectId());
  response.set_x(packet.x());
  response.set_y(packet.y());
  response.set_is_right(packet.is_right());
  response.set_animation(packet.animation());
  BroadCast(response, session);
}

void MapInstance::RespawnMobs() {

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

void MapInstance::OnPlayerEnter(const std::shared_ptr<GameSession>& session) {
  protocol::GameServerAddPlayer add_players;

  for (auto&& other_session : _players | std::views::values) {
    if (other_session == session) {
      continue;
    }

    const auto info = other_session->GetPlayer();
    const auto player_info = add_players.add_player_infos();
    player_info->set_object_id(info->GetObjectId());
    player_info->set_name(utils::ConvertToUtf8(info->GetName()).value());
    player_info->set_type(info->GetType());
    player_info->set_level(info->GetStat()->GetLevel());
    player_info->set_hp(info->GetStat()->GetHp());
    player_info->set_max_hp(info->GetStat()->GetMaxHp());
    player_info->set_x(static_cast<int32_t>(info->GetPosition().x));
    player_info->set_y(static_cast<int32_t>(info->GetPosition().y));
  }

  if (add_players.player_infos_size() > 0) {
    const auto send_buffer = GameClientPacketHandler::MakeSendBuffer(add_players);
    session->Send(send_buffer);
  }

  const auto player = session->GetPlayer();
  protocol::GameServerAddPlayer add_player;
  const auto info = add_player.add_player_infos();
  info->set_object_id(player->GetObjectId());
  info->set_name(utils::ConvertToUtf8(player->GetName()).value());
  info->set_type(player->GetType());
  info->set_level(player->GetStat()->GetLevel());
  info->set_hp(player->GetStat()->GetHp());
  info->set_max_hp(player->GetStat()->GetMaxHp());
  info->set_x(static_cast<int32_t>(player->GetPosition().x));
  info->set_y(static_cast<int32_t>(player->GetPosition().y));

  BroadCast(add_player, player->GetId());
}

void MapInstance::OnPlayerLeave(const std::shared_ptr<Player>& player) {
  protocol::GameServerRemoveObject message;
  message.set_object_id(player->GetObjectId());

  BroadCast(message, player->GetId());
}

int32_t MapInstance::GetMapId() const noexcept {
  return _map_id;
}
