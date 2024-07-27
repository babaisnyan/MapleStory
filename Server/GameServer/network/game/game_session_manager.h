#pragma once
#include "game_session.h"

namespace game {
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
    tbb::concurrent_hash_map<int32_t, GameSessionRef> _game_sessions;

    DECLARE_ACCESSOR_TYPE(_game_sessions);
    DECLARE_CONST_ACCESSOR_TYPE(_game_sessions);
  };
}
