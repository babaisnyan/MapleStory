#include "pch.h"
#include "login_session_manager.h"
#include "login_session.h"

#include <ranges>

void LoginSessionManager::Add(const LoginSessionRef& session) {
  Accessor accessor;

  if (_login_sessions.insert(accessor, session->GetSessionId())) {
    accessor->second = session;
  }
}

void LoginSessionManager::Remove(const LoginSessionRef& session) {
  Accessor accessor;

  if (_login_sessions.find(accessor, session->GetSessionId())) {
    _login_sessions.erase(accessor);
  }
}

void LoginSessionManager::Broadcast(const SendBufferRef& send_buffer) const {
  ConstAccessor accessor;

  for (const auto& session : _login_sessions | std::views::values) {
    session->Send(send_buffer);
  }
}
