#include "pch.h"
#include "game_tick.h"

#include "game/player_stat.h"
#include "game/map/map_instance.h"
#include "game/objects/player/player.h"

#include "network/game/game_client_packet_handler.h"

void GameTick::AddPlayer(const std::shared_ptr<MapInstance>& map, const GameSessionRef& session) {
  protocol::GameServerAddPlayer add_players;
  for (auto&& other_session : map->GetPlayers() | std::views::values) {
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

  const auto player = session->GetPlayer();

  if (map->AddPlayer(session)) {
    protocol::GameServerAddPlayer add_player;
    const auto info = add_player.add_player_infos();
    info->set_object_id(player->GetId());
    info->set_name(utils::ConvertToUtf8(player->GetName()).value());
    info->set_type(player->GetType());
    info->set_level(player->GetStat()->GetLevel());
    info->set_hp(player->GetStat()->GetHp());
    info->set_max_hp(player->GetStat()->GetMaxHp());
    info->set_x(player->GetPosition().x);
    info->set_y(player->GetPosition().y);
    map->BroadCast(add_player, player->GetId());
  }
}

void GameTick::RemovePlayer(const std::shared_ptr<MapInstance>& map, const int64_t object_id) {
  map->RemoveObject(object_id);
}
