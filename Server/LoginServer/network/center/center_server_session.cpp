#include "pch.h"
#include "center_server_session.h"

#include "center_packet_creator.h"
#include "center_server_packet_handler.h"

void CenterServerSession::OnConnected() {
  std::cout << "CenterServerSession Connected\n";

  //TODO: 설정으로 빼기
  const auto name = utils::ConvertToUtf8(L"Login01");
  const auto ip = utils::ConvertToUtf8(L"127.0.0.1");

  if (!name.has_value() || !ip.has_value()) {
    std::cout << "CenterServerSession::OnConnected() failed\n";
    return;
  }

  const auto send_buffer = CenterPacketCreator::GetRegisterRequest(name.value(), ip.value(), 7777);
  Send(send_buffer);
}

void CenterServerSession::OnDisconnected() {
  std::cout << "CenterServerSession Disconnected\n";
}

void CenterServerSession::OnRecvPacket(std::byte* buffer, const int32_t len) {
  PacketSessionRef session = GetPacketSession();
  PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

  CenterServerPacketHandler::HandlePacket(session, buffer, len);
}

void CenterServerSession::OnSend(const int32_t len) {}
