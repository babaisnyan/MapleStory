#pragma once
#include <tbb/concurrent_queue.h>

#include "network/protocol/game_protocol.pb.h"

namespace game {
  class MapInstance;

  class GameTick : public std::enable_shared_from_this<GameTick> {
  public:
    static std::shared_ptr<GameTick>& GetInstance() {
      static auto instance = std::make_shared<GameTick>();
      return instance;
    }

    void Start();

  private:
    std::atomic<bool> _is_running = true;
  };
}
