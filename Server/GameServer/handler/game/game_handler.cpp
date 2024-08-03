#include "pch.h"
#include "game_handler.h"

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

    map.value()->AddPlayer(game_session);
  } else {
    // TODO: 오류 메시지 전송
  }
}
