﻿#include "pch.h"
#include "player_db_queue.h"

#include "game_tick.h"

#include "game/map/map_instance.h"
#include "game/map/map_manager.h"
#include "game/objects/player/player.h"

#include "manager/migration/auth_storage.h"

#include "network/game/game_packet_creator.h"
#include "network/game/game_session.h"

void PlayerDbQueue::HandleClientEnter(const PacketSessionRef& session, const protocol::GameClientEnter& packet) {
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

    const auto map = MapManager::GetInstance().GetMapInstance(player->GetMap());

    if (!map.has_value()) {
      game_session->Disconnect(L"Invalid map");
      return;
    }

    map.value()->DoAsync(&MapInstance::AddPlayer, game_session);
  } else {
    game_session->Disconnect(L"Failed to load player");
  }
}

void PlayerDbQueue::SavePlayer(const PacketSessionRef& session) {
  const auto game_session = std::static_pointer_cast<GameSession>(session);
  const auto player = game_session->GetPlayer();

  if (!player->TrySaveToDb()) {
    session->Disconnect(L"Failed to save player");
  }
}
