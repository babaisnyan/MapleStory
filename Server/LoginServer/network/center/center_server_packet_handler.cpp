#include "pch.h"
#include "center_server_packet_handler.h"

#include "network/login/login_server.h"

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
