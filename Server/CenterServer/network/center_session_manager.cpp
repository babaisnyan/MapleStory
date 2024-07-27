#include "pch.h"
#include "center_session_manager.h"
#include "center_session.h"
#include "data/server_info.h"

#include <ranges>

void CenterSessionManager::Add(const CenterSessionRef& session) {
  Accessor accessor;

  if (_center_sessions.insert(accessor, session->GetServerInfo()->name)) {
    accessor->second = session;
  }
}

void CenterSessionManager::Remove(const String& name) {
  Accessor accessor;

  if (_center_sessions.find(accessor, name)) {
    _center_sessions.erase(accessor);
  }
}

bool CenterSessionManager::IsExist(const String& name) const {
  ConstAccessor accessor;
  return _center_sessions.find(accessor, name);
}

bool CenterSessionManager::IsExist(const String& ip, const uint16_t port) {
  ConstAccessor accessor;

  for (const auto& center_session : _center_sessions | std::views::values) {
    if (center_session->GetServerInfo()->ip == ip && center_session->GetServerInfo()->port == port) {
      return true;
    }
  }

  return false;
}

std::optional<CenterSessionRef> CenterSessionManager::Get(const String& name) const {
  ConstAccessor accessor;
  return _center_sessions.find(accessor, name) ? std::make_optional(accessor->second) : std::nullopt;
}

void CenterSessionManager::Broadcast(const SendBufferRef& send_buffer) {
  ConstAccessor accessor;

  for (const auto& session : _center_sessions | std::views::values) {
    session->Send(send_buffer);
  }
}
