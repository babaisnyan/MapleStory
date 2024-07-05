#include "pch.h"
#include "login_client_packet_handler.h"

#include "../handler/login_manager.h"

bool HandleLoginInvalid(PacketSessionRef& session, std::byte* buffer, const int32_t len) {
  return false;
}

bool HandleLoginClientLogin(const PacketSessionRef& session, const protocol::LoginClientLogin& packet) {
  LoginManager::GetInstance().HandleLoginRequest(session, packet);
  return true;
}
