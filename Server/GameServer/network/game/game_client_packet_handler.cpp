#include "pch.h"
#include "game_client_packet_handler.h"

#include "game_session.h"

#include "game/map/map_instance.h"
#include "game/map/map_manager.h"
#include "game/objects/player/key_map.h"
#include "game/objects/player/player.h"

#include "manager/job_queue/game_tick.h"
#include "manager/job_queue/player_db_queue.h"

namespace game {
  bool HandleGameInvalid(PacketSessionRef& session, std::byte* buffer, const int32_t len) {
    return false;
  }

  bool HandleGameClientEnter(const PacketSessionRef& session, const protocol::GameClientEnter& packet) {
    PlayerDbQueue::GetInstance()->DoAsync(&PlayerDbQueue::HandleClientEnter, session, packet);
    return true;
  }

  bool HandleGameClientPlayerMove(const PacketSessionRef& session, const protocol::GameClientPlayerMove& packet) {
    const auto game_session = std::static_pointer_cast<GameSession>(session);
    const auto player = game_session->GetPlayer();

    if (!player) {
      return false;
    }

    const auto map = MapManager::GetInstance().GetMapInstance(player->GetMap());

    if (!map.has_value()) {
      return false;
    }

    map.value()->DoAsync(&MapInstance::MovePlayer, game_session, packet);
    return true;
  }

  bool HandleGameClientChangeKeySetting(const PacketSessionRef& session, const protocol::GameClientChangeKeySetting& packet) {
    const auto game_session = std::static_pointer_cast<GameSession>(session);
    const auto player = game_session->GetPlayer();

    if (!player) {
      return false;
    }

    const auto map = MapManager::GetInstance().GetMapInstance(player->GetMap());

    if (!map.has_value()) {
      return false;
    }

    map.value()->DoAsync(&MapInstance::OnChangeKeySetting, game_session, packet);
    return true;
  }

  bool HandleGameClientChat(const PacketSessionRef& session, const protocol::GameClientChat& packet) {
    const auto game_session = std::static_pointer_cast<GameSession>(session);
    const auto player = game_session->GetPlayer();

    if (!player) {
      return false;
    }

    const auto map = MapManager::GetInstance().GetMapInstance(player->GetMap());

    if (!map.has_value()) {
      return false;
    }

    map.value()->DoAsync(&MapInstance::OnChat, game_session, packet);
    return true;
  }

  bool HandleGameClientRevive(const PacketSessionRef& session, const protocol::GameClientRevive& packet) {
    const auto game_session = std::static_pointer_cast<GameSession>(session);
    const auto player = game_session->GetPlayer();

    if (!player) {
      return false;
    }

    const auto map = MapManager::GetInstance().GetMapInstance(player->GetMap());

    if (!map.has_value()) {
      return false;
    }

    map.value()->DoAsync(&MapInstance::OnRevive, game_session, player);
    return true;
  }

  bool HandleGameClientAttack(const PacketSessionRef& session, const protocol::GameClientAttack& packet) {
    const auto game_session = std::static_pointer_cast<GameSession>(session);
    const auto player = game_session->GetPlayer();

    if (!player) {
      return false;
    }

    const auto map = MapManager::GetInstance().GetMapInstance(player->GetMap());

    if (!map.has_value()) {
      return false;
    }

    map.value()->DoAsync(&MapInstance::OnAttack, game_session, player, packet);

    return true;
  }

  bool HandleGameClientMoveInventory(const PacketSessionRef& session, const protocol::GameClientMoveInventory& packet) {
    const auto game_session = std::static_pointer_cast<GameSession>(session);
    const auto player = game_session->GetPlayer();

    if (!player) {
      return false;
    }

    const auto map = MapManager::GetInstance().GetMapInstance(player->GetMap());

    if (!map.has_value()) {
      return false;
    }

    map.value()->DoAsync(&MapInstance::OnMoveItem, player, packet);

    return true;
  }

  bool HandleGameClientUseItem(const PacketSessionRef& session, const protocol::GameClientUseItem& packet) {
    const auto game_session = std::static_pointer_cast<GameSession>(session);
    const auto player = game_session->GetPlayer();

    if (!player) {
      return false;
    }

    const auto map = MapManager::GetInstance().GetMapInstance(player->GetMap());

    if (!map.has_value()) {
      return false;
    }

    map.value()->DoAsync(&MapInstance::OnUseItem, player, packet);

    return true;
  }

  bool HandleGameClientEquipItem(const PacketSessionRef& session, const protocol::GameClientEquipItem& packet) {
    return true;
  }

  bool HandleGameClientUnequipItem(const PacketSessionRef& session, const protocol::GameClientUnequipItem& packet) {
    return true;
  }
}
