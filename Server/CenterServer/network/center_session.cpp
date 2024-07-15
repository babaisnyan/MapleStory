#include "pch.h"
#include "center_session.h"

#include "PacketCreator.h"
#include "center_client_packet_handler.h"
#include "center_session_manager.h"


void CenterSession::OnConnected() {
  const auto session = std::static_pointer_cast<CenterSession>(shared_from_this());
  CenterSessionManager::GetInstance().Add(session);

  const auto packet = PacketCreator::GetHandshake();
  session->Send(packet);
}

void CenterSession::OnDisconnected() {
  CenterSessionManager::GetInstance().Remove(std::static_pointer_cast<CenterSession>(shared_from_this()));

  std::cout << "CenterSession Disconnected\n";
}

void CenterSession::OnRecvPacket(std::byte* buffer, const int32_t len) {
  PacketSessionRef session = GetPacketSession();
  PacketHeader*    header = reinterpret_cast<PacketHeader*>(buffer);

  CenterClientPacketHandler::HandlePacket(session, buffer, len);
}

void CenterSession::OnSend(const int32_t len) {}
