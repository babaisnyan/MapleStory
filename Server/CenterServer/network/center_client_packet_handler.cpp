﻿#include "pch.h"
#include "center_client_packet_handler.h"

#include "handler/center_manager.h"

bool HandleCenterInvalid(PacketSessionRef& session, std::byte* buffer, const int32_t len) {
  return false;
}

bool HandleCenterClientRegisterRequest(const PacketSessionRef& session, const protocol::CenterClientRegisterRequest& packet) {
  CenterManager::GetInstance().HandleServerRegisterRequest(session, packet);
  return true;
}

bool HandleCenterClientMigrationRequest(const PacketSessionRef& session, const protocol::CenterClientMigrationRequest& packet) {
  CenterManager::GetInstance().HandleServerMigrationRequest(session, packet);
  return true;
}