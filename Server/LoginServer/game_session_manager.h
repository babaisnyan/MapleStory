#pragma once

class GameSession;

using GameSessionRef = std::shared_ptr<GameSession>;

class GameSessionManager {
public:
  static GameSessionManager& GetInstance() {
    static GameSessionManager instance;
    return instance;
  }

public:
  void Add(const GameSessionRef& session);
  void Remove(const GameSessionRef& session);

  void Broadcast(const SendBufferRef& send_buffer);

private:
  USE_LOCK;
  Set<GameSessionRef> _game_sessions;
};