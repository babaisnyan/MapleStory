#include "pch.h"
#include "center_session_manager.h"

#include <ranges>

#include "center_session.h"
#include "data/server_info.h"

void CenterSessionManager::Add(const CenterSessionRef& session, const String& name) {
  WRITE_LOCK;
  _center_sessions[name] = session;

  auto server_info = std::make_shared<ServerInfo>(name, session->GetServerIp(), session->GetServerPort());
  _server_infos[name] = std::move(server_info);
}

void CenterSessionManager::Remove(const CenterSessionRef& session) {
  WRITE_LOCK;
  std::optional<String> found_name = std::nullopt;

  for (const auto& [name, center_session] : _center_sessions) {
    if (center_session == session) {
      found_name = name;
      break;
    }
  }

  if (found_name.has_value()) {
    _center_sessions.erase(found_name.value());
  }
}

void CenterSessionManager::RemoveByName(const String& name) {
  WRITE_LOCK;
  _center_sessions.erase(name);
}

bool CenterSessionManager::IsExist(const String& name) {
  READ_LOCK;
  return _center_sessions.contains(name);
}

bool CenterSessionManager::IsExist(const String& ip, const uint16_t port) {
  READ_LOCK;

  for (const auto& center_session : _center_sessions | std::views::values) {
    if (center_session->GetServerIp() == ip && center_session->GetServerPort() == port) {
      return true;
    }
  }

  return false;
}

CenterSessionRef CenterSessionManager::Get(const String& name) {
  READ_LOCK;
  return _center_sessions.at(name);
}

void CenterSessionManager::Broadcast(const SendBufferRef& send_buffer) {
  WRITE_LOCK;

  for (const auto& session : _center_sessions | std::views::values) {
    session->Send(send_buffer);
  }
}

std::optional<std::shared_ptr<ServerInfo>> CenterSessionManager::GetServerInfo(const String& name) {
  READ_LOCK;

  if (_server_infos.contains(name)) {
    return _server_infos[name];
  }

  return std::nullopt;
}
