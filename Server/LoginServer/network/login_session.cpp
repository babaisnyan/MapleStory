#include "pch.h"
#include "login_session.h"
#include "login_client_packet_handler.h"
#include "login_session_manager.h"


void LoginSession::OnConnected() {
  CenterSessionManager::GetInstance().Add(std::static_pointer_cast<LoginSession>(shared_from_this()));
}

void LoginSession::OnDisconnected() {
  CenterSessionManager::GetInstance().Remove(std::static_pointer_cast<LoginSession>(shared_from_this()));

  std::cout << "LoginSession Disconnected\n";
}

void LoginSession::OnRecvPacket(std::byte* buffer, const int32_t len) {
  PacketSessionRef session = GetPacketSession();
  PacketHeader*    header = reinterpret_cast<PacketHeader*>(buffer);

  LoginClientPacketHandler::HandlePacket(session, buffer, len);
}

void LoginSession::OnSend(const int32_t len) {}
