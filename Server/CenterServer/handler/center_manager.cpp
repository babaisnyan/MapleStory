#include "pch.h"
#include "center_manager.h"

void CenterManager::HandleServerRegisterRequest(const PacketSessionRef& session, const protocol::CenterClientRegisterRequest& request) {
  _center_handler->DoAsync(&CenterHandler::HandleServerRegisterRequest, session, request);
}

void CenterManager::HandleServerMigrationRequest(const PacketSessionRef& session, const protocol::CenterClientMigrationRequest& request) {
  _center_handler->DoAsync(&CenterHandler::HandleServerMigrationRequest, session, request);
}

void CenterManager::HandleServerMigrationResponse(const PacketSessionRef& session, const protocol::CenterClientMigrationResponse& response) {
  _center_handler->DoAsync(&CenterHandler::HandleServerMigrationResponse, session, response);
}
