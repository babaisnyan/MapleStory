#include "pch.h"
#include "game_handler.h"

#include "game/player_stat.h"
#include "game/map/map_instance.h"
#include "game/map/map_manager.h"
#include "game/objects/player/player.h"

#include "migration/auth_storage.h"

#include "network/game/game_packet_creator.h"

void GameHandler::HandleClientEnter(PacketSessionRef session, protocol::GameClientEnter packet) {
  const auto auth_key = AuthStorage::GetInstance().Find(packet.character_id());
  const auto game_session = std::static_pointer_cast<GameSession>(session);

  if (!auth_key.has_value()) {
    const auto response = GamePacketCreator::GetClientEnterFailResponse();
    game_session->Send(response);
    return;
  }

  if (auth_key.value() != packet.auth_key()) {
    const auto response = GamePacketCreator::GetClientEnterFailResponse();
    game_session->Send(response);
    return;
  }


  AuthStorage::GetInstance().Remove(packet.character_id());
  const auto player = std::make_shared<Player>(packet.character_id());

  if (player->TryLoadFromDb()) {
    game_session->SetPlayer(player);
    const auto response = GamePacketCreator::GetClientEnterSuccessResponse(player);
    game_session->Send(response);
    player->OnEnter();

    //TODO: 게임 로직 업데이트 함수에서 처리하도록 변경
    const auto map = MapManager::GetInstance().GetMapInstance(player->GetMap());

    if (!map.has_value()) {
      // TODO: 접속 끊기
      return;
    }

    protocol::GameServerAddPlayer add_players;

    for (auto&& other_session : map.value()->GetPlayers() | std::views::values) {
      const auto info = other_session->GetPlayer();
      const auto player_info = add_players.add_player_infos();
      player_info->set_id(info->GetId());
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
      game_session->Send(send_buffer);
    }

    if (map.value()->AddPlayer(game_session)) {
      protocol::GameServerAddPlayer add_player;
      const auto info = add_player.add_player_infos();
      info->set_id(player->GetId());
      info->set_name(utils::ConvertToUtf8(player->GetName()).value());
      info->set_type(player->GetType());
      info->set_level(player->GetStat()->GetLevel());
      info->set_hp(player->GetStat()->GetHp());
      info->set_max_hp(player->GetStat()->GetMaxHp());
      info->set_x(player->GetPosition().x);
      info->set_y(player->GetPosition().y);
      map.value()->BroadCast(add_player, player->GetId());
    }
  } else {
    // TODO: 오류 메시지 전송
  }
}
