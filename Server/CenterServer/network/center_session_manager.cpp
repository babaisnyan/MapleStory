#include "pch.h"
#include "center_session_manager.h"

#include "center_session.h"

void CenterSessionManager::Add(const CenterSessionRef& session) {
  WRITE_LOCK;
  _center_sessions.insert(session);
}

void CenterSessionManager::Remove(const CenterSessionRef& session) {
  WRITE_LOCK;
  _center_sessions.erase(session);
}

void CenterSessionManager::Broadcast(const SendBufferRef& send_buffer) {
  WRITE_LOCK;

  for (const CenterSessionRef& center_session : _center_sessions) {
    center_session->Send(send_buffer);
  }
}
