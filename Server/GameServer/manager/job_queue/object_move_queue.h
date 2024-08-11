#pragma once

#include "job/job_queue.h"

#include "network/protocol/game_protocol.pb.h"

namespace game {
  class ObjectMoveQueue : public JobQueue {
  public:
    static std::shared_ptr<ObjectMoveQueue>& GetInstance() {
      static auto instance = std::make_shared<ObjectMoveQueue>();
      return instance;
    }

  public:
    void MovePlayer(const PacketSessionRef& session, const protocol::GameClientPlayerMove& packet);
  };
}
