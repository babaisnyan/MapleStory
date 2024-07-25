#include "pch.h"
#include "center_handler.h"

#include "network/center/center_packet_creator.h"

#include "utils/randomizer.h"

void CenterHandler::HandleMigrationResponse(PacketSessionRef session, protocol::CenterServerMigrationResponse packet) {}

void CenterHandler::HandleMigrationRequest(PacketSessionRef session, protocol::CenterServerMigrationRequest packet) {
  //TODO: 현재 채널에 접속 가능한 플레이어인지 검증, 인증 리스트에 캐릭터 아이디 추가

  const auto center_session = std::static_pointer_cast<CenterServerSession>(session);
  const auto auth_key = utils::Randomizer::GetInstance().GetRandomInt32(0x12345678, 0x54321012);
  const auto response = CenterPacketCreator::GetMigrationSuccessResponse(packet.character_id(), auth_key);

  center_session->Send(response);
}
