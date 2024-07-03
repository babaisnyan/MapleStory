#include "pch.h"
#include "game_session_manager.h"

#include "game_session.h"

void GameSessionManager::Add(const GameSessionRef& session) {
  WRITE_LOCK;
  _game_sessions.insert(session);
}

void GameSessionManager::Remove(const GameSessionRef& session) {
  WRITE_LOCK;
  _game_sessions.erase(session);
}

void GameSessionManager::Broadcast(const SendBufferRef& send_buffer) {
  WRITE_LOCK;

  for (const GameSessionRef& game_session : _game_sessions) {
    game_session->Send(send_buffer);
  }
}
