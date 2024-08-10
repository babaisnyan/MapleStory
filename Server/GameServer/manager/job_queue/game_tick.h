#pragma once
#include "job/job_queue.h"

namespace game {
  class MapInstance;

  class GameTick : public JobQueue {
  public:
    static std::shared_ptr<GameTick>& GetInstance() {
      static auto instance = std::make_shared<GameTick>();
      return instance;
    }

  public:
    void AddPlayer(const std::shared_ptr<MapInstance>& map, const GameSessionRef& session);
    void RemovePlayer(const std::shared_ptr<MapInstance>& map, int64_t object_id);
  };
}
