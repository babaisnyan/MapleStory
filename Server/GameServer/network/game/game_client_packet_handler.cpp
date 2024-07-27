#include "pch.h"
#include "game_client_packet_handler.h"

#include "handler/game/game_manager.h"

namespace game {
  bool HandleGameInvalid(PacketSessionRef& session, std::byte* buffer, const int32_t len) {
    return false;
  }

  bool HandleGameClientEnter(const PacketSessionRef& session, const protocol::GameClientEnter& packet) {
    GameManager::GetInstance().HandleClientEnter(session, packet);
    return true;
  }
}
