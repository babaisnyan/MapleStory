#include "pch.h"
#include "center_server_packet_handler.h"

bool HandleCenterInvalid(PacketSessionRef& session, std::byte* buffer, const int32_t len) {
  return false;
}

bool HandleCenterServerHandshake(const PacketSessionRef& session, const protocol::CenterServerHandshake& packet) {
  return true;
}

bool HandleCenterServerRegisterResponse(const PacketSessionRef& session, const protocol::CenterServerRegisterResponse& packet) {
  return true;
}
