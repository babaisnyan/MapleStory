#include "pch.h"
#include "center_handler.h"

#include "migration/migration_storage.h"

#include "network/login/login_packet_creator.h"
#include "network/login/login_session.h"

void CenterHandler::HandleMigrationResponse(PacketSessionRef session, protocol::CenterServerMigrationResponse packet) {
  const auto login_session_weak = MigrationStorage::GetInstance().Find(packet.character_id());

  if (!login_session_weak.has_value()) {
    return;
  }

  MigrationStorage::GetInstance().Remove(packet.character_id());

  const auto login_session = login_session_weak.value().lock();
  if (!login_session) {
    return;
  }

  if (packet.success()) {
    const auto response = LoginPacketCreator::GetSelectCharSuccessResponse(packet.server().ip(), packet.server().port(), packet.auth_key());
    login_session->Send(response);
  } else {
    const auto response = LoginPacketCreator::GetSelectCharFailedResponse();
    login_session->Send(response);
  }
}

void CenterHandler::HandleMigrationRequest(PacketSessionRef session, protocol::CenterServerMigrationRequest packet) {}
