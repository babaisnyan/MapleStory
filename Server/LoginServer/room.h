#pragma once
#include "job/job_queue.h"

class Room : public JobQueue {
public:
  void Enter(PlayerRef player);
  void Leave(PlayerRef player);
  void Broadcast(SendBufferRef send_buffer);

private:
  std::map<uint64_t, PlayerRef> _players;
};

extern std::shared_ptr<Room> GRoom;
