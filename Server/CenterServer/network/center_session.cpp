#include "pch.h"
#include "center_session.h"

#include "center_client_packet_handler.h"
#include "center_session_manager.h"
#include "packet_creator.h"


void CenterSession::RegisterServer(const String& server_name, const String& server_ip, const uint16_t server_port) {
  _server_name = server_name;
  _server_ip = server_ip;
  _server_port = server_port;
  CenterSessionManager::GetInstance().Add(std::static_pointer_cast<CenterSession>(shared_from_this()), server_name);
  _is_registered = true;
}

void CenterSession::OnConnected() {
  std::cout << "CenterSession Connected\n";
}

void CenterSession::OnDisconnected() {
  if (_is_registered) {
    CenterSessionManager::GetInstance().Remove(std::static_pointer_cast<CenterSession>(shared_from_this()));
  }

  std::cout << "CenterSession Disconnected\n";
}

void CenterSession::OnRecvPacket(std::byte* buffer, const int32_t len) {
  PacketSessionRef session = GetPacketSession();
  PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

  CenterClientPacketHandler::HandlePacket(session, buffer, len);
}

void CenterSession::OnSend(const int32_t len) {}
