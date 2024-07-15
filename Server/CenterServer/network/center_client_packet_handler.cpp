#include "pch.h"
#include "center_client_packet_handler.h"

bool HandleCenterInvalid(PacketSessionRef& session, std::byte* buffer, const int32_t len) {
  return false;
}

bool HandleCenterClientRegisterRequest(const PacketSessionRef& session, const protocol::CenterClientRegisterRequest& packet) {
  return true;
}
