#include "pch.h"
#include "center_manager.h"

void CenterManager::HandleMigrationResponse(const PacketSessionRef& session, const protocol::CenterServerMigrationResponse& packet) {
  _center_handler->DoAsync(&CenterHandler::HandleMigrationResponse, session, packet);
}

void CenterManager::HandleMigrationRequest(const PacketSessionRef& session, const protocol::CenterServerMigrationRequest& packet) {
  _center_handler->DoAsync(&CenterHandler::HandleMigrationRequest, session, packet);
}
