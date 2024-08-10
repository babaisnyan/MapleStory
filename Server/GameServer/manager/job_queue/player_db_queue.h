#pragma once
#include "job/job_queue.h"

#include "network/protocol/game_protocol.pb.h"

namespace game {
  class PlayerDbQueue : public JobQueue {
  public:
    static std::shared_ptr<PlayerDbQueue>& GetInstance() {
      static auto instance = std::make_shared<PlayerDbQueue>();
      return instance;
    }

  public:
    void HandleClientEnter(const PacketSessionRef& session, const protocol::GameClientEnter& packet);
    void SavePlayer(const PacketSessionRef& session);
  };
}
