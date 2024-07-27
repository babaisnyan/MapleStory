#include "pch.h"
#include "center_handler.h"

#include "migration/auth_storage.h"

#include "network/center/center_packet_creator.h"

#include "utils/randomizer.h"

void CenterHandler::HandleMigrationResponse(PacketSessionRef session, protocol::CenterServerMigrationResponse packet) {}

void CenterHandler::HandleMigrationRequest(PacketSessionRef session, protocol::CenterServerMigrationRequest packet) {
  //TODO: 중복 플레이어 검사

  const auto center_session = std::static_pointer_cast<CenterServerSession>(session);
  const auto auth_key = utils::Randomizer::GetInstance().GetRandomInt32(0x12345678, 0x54321012);

  if (AuthStorage::GetInstance().IsExist(packet.character_id())) {
    const auto response = CenterPacketCreator::GetMigrationFailedResponse(packet.character_id());
    center_session->Send(response);
    return;
  }

  AuthStorage::GetInstance().Add(packet.character_id(), auth_key);

  const auto response = CenterPacketCreator::GetMigrationSuccessResponse(packet.character_id(), auth_key);
  center_session->Send(response);
}
