#include "pch.h"
#include "game_client_packet_handler.h"

#include "manager/job_queue/player_db_queue.h"

namespace game {
  bool HandleGameInvalid(PacketSessionRef& session, std::byte* buffer, const int32_t len) {
    return false;
  }

  bool HandleGameClientEnter(const PacketSessionRef& session, const protocol::GameClientEnter& packet) {
    PlayerDbQueue::GetInstance()->DoAsync(&PlayerDbQueue::HandleClientEnter, session, packet);
    return true;
  }

  bool HandleGameClientPlayerMove(const PacketSessionRef& session, const protocol::GameClientPlayerMove& packet) {
    return true;
  }
}
