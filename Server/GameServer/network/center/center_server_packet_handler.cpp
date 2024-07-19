#include "pch.h"
#include "center_server_packet_handler.h"

bool HandleCenterInvalid(PacketSessionRef& session, std::byte* buffer, const int32_t len) {
  return false;
}

bool HandleCenterServerRegisterResponse(const PacketSessionRef& session, const protocol::CenterServerRegisterResponse& packet) {
  return true;
}

bool HandleCenterServerMigrationResponse(const PacketSessionRef& session, const protocol::CenterServerMigrationResponse& packet) {
  return true;
}

bool HandleCenterServerMigrationRequest(const PacketSessionRef& session, const protocol::CenterServerMigrationRequest& packet) {
  return true;
}
