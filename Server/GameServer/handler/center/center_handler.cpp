#include "pch.h"
#include "center_handler.h"

#include "game/player_manager.h"

#include "manager/migration/auth_storage.h"

#include "network/center/center_packet_creator.h"

#include "utils/randomizer.h"

void CenterHandler::HandleMigrationResponse(PacketSessionRef session, protocol::CenterServerMigrationResponse packet) {}

void CenterHandler::HandleMigrationRequest(const PacketSessionRef& session, const protocol::CenterServerMigrationRequest& packet) {
  if (PlayerManager::GetInstance().Find(packet.character_id()).has_value()) {
    const auto response = CenterPacketCreator::GetMigrationFailedResponse(packet.character_id());
    session->Send(response);
    return;
  }

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
