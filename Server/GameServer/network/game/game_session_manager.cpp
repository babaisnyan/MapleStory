#include "pch.h"
#include "game_session_manager.h"

#include <ranges>

#include "game_session.h"

void GameSessionManager::Add(const GameSessionRef& session) {
  Accessor accessor;

  if (_game_sessions.insert(accessor, session->GetSessionId())) {
    accessor->second = session;
  }
}

void GameSessionManager::Remove(const GameSessionRef& session) {
  Accessor accessor;

  if (_game_sessions.find(accessor, session->GetSessionId())) {
    _game_sessions.erase(accessor);
  }
}

void GameSessionManager::Broadcast(const SendBufferRef& send_buffer) {
  ConstAccessor accessor;

  for (const auto& game_session : _game_sessions | std::views::values) {
    game_session->Send(send_buffer);
  }
}
