#include "pch.h"
#include "center_server_packet_handler.h"

#include "auth/migration_storage.h"

#include "network/login/login_packet_creator.h"
#include "network/login/login_server.h"
#include "network/login/login_session.h"

bool HandleCenterInvalid(PacketSessionRef& session, std::byte* buffer, const int32_t len) {
  return false;
}

bool HandleCenterServerRegisterResponse(const PacketSessionRef& session, const protocol::CenterServerRegisterResponse& packet) {
  switch (packet.result()) {
    case protocol::SERVER_REGISTER_RESULT_SUCCESS:
      LoginServer::GetInstance().StartLoginServer();
      break;
    case protocol::SERVER_REGISTER_RESULT_ALREADY_REGISTERED:
      break;
    case protocol::SERVER_REGISTER_RESULT_INVALID_TYPE:
      break;
    case protocol::SERVER_REGISTER_RESULT_INVALID_VERSION:
      break;
    case protocol::SERVER_REGISTER_RESULT_INVALID_NAME:
      break;
    case protocol::SERVER_REGISTER_RESULT_ERROR:
      break;
    default:
      break;
  }

  return true;
}

bool HandleCenterServerMigrationResponse(const PacketSessionRef& session, const protocol::CenterServerMigrationResponse& packet) {
  const auto login_session_weak = MigrationStorage::GetInstance().Find(packet.character_id());

  if (!login_session_weak.has_value()) {
    return false;
  }

  const auto login_session = login_session_weak.value().lock();
  if (!login_session) {
    return false;
  }

  const auto response = LoginPacketCreator::GetSelectCharSuccessResponse("", 1);
  login_session->Send(response);
  return true;
}

bool HandleCenterServerMigrationRequest(const PacketSessionRef& session, const protocol::CenterServerMigrationRequest& packet) {
  return true;
}
