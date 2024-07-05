#include "pch.h"
#include "login_session_manager.h"

#include "login_session.h"

void LoginSessionManager::Add(const LoginSessionRef& session) {
  WRITE_LOCK;
  _login_sessions.insert(session);
}

void LoginSessionManager::Remove(const LoginSessionRef& session) {
  WRITE_LOCK;
  _login_sessions.erase(session);
}

void LoginSessionManager::Broadcast(const SendBufferRef& send_buffer) {
  WRITE_LOCK;

  for (const LoginSessionRef& login_session : _login_sessions) {
    login_session->Send(send_buffer);
  }
}
