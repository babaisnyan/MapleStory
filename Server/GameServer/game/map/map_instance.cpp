﻿#include "pch.h"
#include "map_instance.h"

#include "game/player_stat.h"
#include "game/objects/mob/monster.h"
#include "game/objects/player/player.h"

MapInstance::MapInstance(const int32_t map_id, const std::pair<int32_t, int32_t> size, std::vector<GroundInfo> grounds, std::tuple<int32_t, int32_t, int32_t, int32_t> bounds) :
  _map_id(map_id),
  _size(size),
  _bounds(std::move(bounds)),
  _grounds(std::move(grounds)) {
  _max_grid_x = _size.first / MsCoordinate::kGridSize + 1;
  _max_grid_y = _size.second / MsCoordinate::kGridSize + 1;

  _grid = std::vector(_max_grid_y, std::vector(_max_grid_x, std::vector<std::shared_ptr<GameObject>>()));

  for (auto& row : _grid) {
    for (auto& column : row) {
      column.reserve(10);
    }
  }
}

bool MapInstance::AddPlayer(const std::shared_ptr<GameSession>& session) {
  const auto inserted = _players.emplace(session->GetPlayer()->GetId(), session).second;

  if (inserted) {
    _objects.emplace(session->GetPlayer()->GetObjectId(), session->GetPlayer());
    OnPlayerEnter(session);
    session->GetPlayer()->SetBounds(_bounds);

    const auto& position = session->GetPlayer()->GetPosition();
    _grid[position.grid_y][position.grid_x].push_back(session->GetPlayer());
  }

  return inserted;
}

bool MapInstance::RemoveObject(const int64_t object_id) {
  const auto type = static_cast<GameObject::ObjectType>(object_id / GameObject::kObjectRange);
  const auto object = _objects.find(object_id);

  if (object != _objects.end()) {
    const auto& position = object->second->GetPosition();

    std::erase(_grid[position.grid_y][position.grid_x], object->second);
  }

  if (type == GameObject::ObjectType::kPlayer) {
    const auto player_session = GetPlayer(object_id);

    if (!player_session.has_value()) {
      return false;
    }

    const auto player = player_session.value()->GetPlayer();
    const auto player_id = player->GetId();

    if (_objects.erase(object_id) <= 0 || _players.erase(player_id) <= 0) {
      return false;
    }

    OnPlayerLeave(player);
  }

  return true;
}

void MapInstance::MovePlayer(const std::shared_ptr<GameSession>& session, const protocol::GameClientPlayerMove& packet) {
  const auto player = session->GetPlayer();

  if (!player) {
    return;
  }

  auto& position = player->GetPosition();
  const auto old_x = position.grid_x;
  const auto old_y = position.grid_y;
  player->UpdatePosition(packet.x(), packet.y(), packet.flip());

  if (old_x != position.grid_x || old_y != position.grid_y) {
    // std::cout << std::format("Player {} moved from ({}, {}) to ({}, {})\n", player->GetId(), old_x, old_y, position.grid_x, position.grid_y);
    std::erase(_grid[old_y][old_x], player);
    _grid[position.grid_y][position.grid_x].push_back(player);
  }

  protocol::GameServerPlayerMove response;
  response.set_object_id(player->GetObjectId());
  response.set_x(packet.x());
  response.set_y(packet.y());
  response.set_flip(packet.flip());
  response.set_animation(packet.animation());
  BroadCast(response, session);
}

void MapInstance::MoveObject(const std::shared_ptr<GameObject>& object, const int16_t old_x, const int16_t old_y) {
  if (!object) {
    return;
  }

  const auto& position = object->GetPosition();
  std::erase(_grid[old_y][old_x], object);
  _grid[position.grid_y][position.grid_x].push_back(object);
}

void MapInstance::Update(const float delta) {
  if (_players.empty()) {
    return;
  }

  RespawnMobs();

  for (const auto& object : _objects | std::ranges::views::values) {
    object->Update(delta);
  }
}

void MapInstance::RespawnMobs() {
  if (_last_respawn_tick + 5000 > GetTickCount64()) {
    return;
  }

  auto map = std::static_pointer_cast<MapInstance>(shared_from_this());
  std::vector<std::shared_ptr<Monster>> temp_mobs;
  protocol::GameServerAddMonster packet;

  for (const auto& [spawn, mob_template] : _mob_spawn_locations) {
    const auto it = _mobs.find(spawn);

    if (it == _mobs.end()) {
      auto mob = std::make_shared<Monster>(spawn, map);
      mob->Init(mob_template);

      const auto entry = packet.add_mob_infos();
      entry->set_id(spawn->GetId());
      entry->set_object_id(mob->GetObjectId());
      entry->set_x(spawn->GetX());
      entry->set_y(spawn->GetY());
      entry->set_state(protocol::MOB_ACTION_TYPE_STAND);
      entry->set_flip(mob->IsFlipped());

      _mobs.emplace(spawn, mob);
      _objects.emplace(mob->GetObjectId(), mob);
      temp_mobs.push_back(mob);
    }
  }

  if (!temp_mobs.empty()) {
    BroadCast(packet, nullptr);

    for (const auto& mob : temp_mobs) {
      const auto position = mob->GetPosition();
      _grid[position.grid_y][position.grid_x].push_back(mob);
      mob->OnEnter();
    }

    _last_respawn_tick = GetTickCount64();
  }
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
    player_info->set_x(info->GetPosition().x);
    player_info->set_y(info->GetPosition().y);
  }

  if (add_players.player_infos_size() > 0) {
    const auto send_buffer = GameClientPacketHandler::MakeSendBuffer(add_players);
    session->Send(send_buffer);
  }

  protocol::GameServerAddMonster mobs;

  for (const auto& mob : _mobs | std::views::values) {
    const auto entry = mobs.add_mob_infos();
    entry->set_id(mob->GetId());
    entry->set_object_id(mob->GetObjectId());
    entry->set_x(mob->GetX());
    entry->set_y(mob->GetY());
    entry->set_state(mob->GetCurrentState());
    entry->set_flip(mob->IsFlipped());

    if (mob->HasTargetPosition()) {
      const auto target_position = mob->GetTargetPosition();
      entry->set_target_x(target_position->x);
      entry->set_target_y(target_position->y);
    }
  }

  if (mobs.mob_infos_size() > 0) {
    const auto send_buffer = GameClientPacketHandler::MakeSendBuffer(mobs);
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
  info->set_x(player->GetPosition().x);
  info->set_y(player->GetPosition().y);

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

std::pair<int32_t, int32_t> MapInstance::GetSize() const noexcept {
  return _size;
}

std::tuple<int32_t, int32_t, int32_t, int32_t> MapInstance::GetBounds() const noexcept {
  return _bounds;
}

std::pair<int32_t, int32_t> MapInstance::GetGridSize() const noexcept {
  return {_max_grid_x, _max_grid_y};
}

const std::vector<std::shared_ptr<GameObject>>& MapInstance::GetObjects(const int16_t x, const int16_t y) noexcept {
  return _grid[y][x];
}

void MapInstance::AddSpawnLocation(const std::shared_ptr<SpawnPoint>& spawn_point, const std::shared_ptr<MobTemplate>& mob_template) {
  _mob_spawn_locations.emplace(spawn_point, mob_template);
}
