#include "pch.h"
#include "login_session.h"
#include "login_client_packet_handler.h"
#include "login_session_manager.h"

#include "migration/migration_storage.h"


void LoginSession::OnConnected() {
  LoginSessionManager::GetInstance().Add(std::static_pointer_cast<LoginSession>(shared_from_this()));
}

void LoginSession::OnDisconnected() {
  const auto session = std::static_pointer_cast<LoginSession>(shared_from_this());
  LoginSessionManager::GetInstance().Remove(session);
  MigrationStorage::GetInstance().Remove(session);
  std::cout << "LoginSession Disconnected\n";
}

void LoginSession::OnRecvPacket(std::byte* buffer, const int32_t len) {
  PacketSessionRef session = GetPacketSession();
  PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

  LoginClientPacketHandler::HandlePacket(session, buffer, len);
}

void LoginSession::OnSend(const int32_t len) {}
