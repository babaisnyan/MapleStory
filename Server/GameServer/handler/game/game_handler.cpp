#include "pch.h"
#include "game_handler.h"

#include "game/player_manager.h"

#include "migration/auth_storage.h"

#include "network/game/game_packet_creator.h"

void GameHandler::HandleClientEnter(PacketSessionRef session, protocol::GameClientEnter packet) {
  const auto auth_key = AuthStorage::GetInstance().Find(packet.character_id());
  const auto game_session = std::static_pointer_cast<GameSession>(session);

  if (!auth_key.has_value()) {
    const auto response = GamePacketCreator::GetClientEnterResponse(false);
    game_session->Send(response);
    return;
  }

  if (auth_key.value() != packet.auth_key()) {
    const auto response = GamePacketCreator::GetClientEnterResponse(false);
    game_session->Send(response);
    return;
  }

  const auto response = GamePacketCreator::GetClientEnterResponse(true);
  game_session->Send(response);
  AuthStorage::GetInstance().Remove(packet.character_id());

  PlayerManager::GetInstance().AddPlayer(packet.character_id(), game_session);
  game_session->SetPlayerId(packet.character_id());
}
