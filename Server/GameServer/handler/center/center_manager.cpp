#include "pch.h"
#include "center_manager.h"

void CenterManager::HandleMigrationResponse(const PacketSessionRef& session, const protocol::CenterServerMigrationResponse& packet) {
  //TODO: 채널 이동 처리
}

void CenterManager::HandleMigrationRequest(const PacketSessionRef& session, const protocol::CenterServerMigrationRequest& packet) {
  _center_handler->DoAsync(&CenterHandler::HandleMigrationRequest, session, packet);
}
