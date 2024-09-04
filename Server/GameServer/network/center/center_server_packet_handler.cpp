#include "pch.h"
#include "center_server_packet_handler.h"

#include "handler/center/center_manager.h"

#include "network/game/game_server.h"

namespace game {
  bool HandleCenterInvalid(PacketSessionRef& session, std::byte* buffer, const int32_t len) {
    return false;
  }

  bool HandleCenterServerRegisterResponse(const PacketSessionRef& session, const protocol::CenterServerRegisterResponse& packet) {
    if (packet.result() == protocol::SERVER_REGISTER_RESULT_SUCCESS) {
      GameServer::GetInstance().StartGameServer();
    }

    return true;
  }

  bool HandleCenterServerMigrationResponse(const PacketSessionRef& session, const protocol::CenterServerMigrationResponse& packet) {
    CenterManager::GetInstance().HandleMigrationResponse(session, packet);
    return true;
  }

  bool HandleCenterServerMigrationRequest(const PacketSessionRef& session, const protocol::CenterServerMigrationRequest& packet) {
    CenterManager::GetInstance().HandleMigrationRequest(session, packet);
    return true;
  }
}
