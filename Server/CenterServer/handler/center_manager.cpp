﻿#include "pch.h"
#include "center_manager.h"

void CenterManager::HandleServerRegisterRequest(const PacketSessionRef& session, const protocol::CenterClientRegisterRequest& request) {
  _center_handler->DoAsync(&CenterHandler::HandleServerRegisterRequest, session, request);
}

void CenterManager::HandleServerMigrationRequest(const PacketSessionRef& session, const protocol::CenterClientMigrationRequest& request) {
  _center_handler->DoAsync(&CenterHandler::HandleServerMigrationRequest, session, request);
}