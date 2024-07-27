#pragma once
#include "job/job_queue.h"

#include "network/game/game_client_packet_handler.h"
#include "network/game/game_session.h"
#include "network/protocol/game_protocol.pb.h"

namespace game {
  class GameHandler final : public JobQueue {
  public:
    template <typename T> requires std::is_base_of_v<google::protobuf::Message, T>
    void SendPacket(const GameSessionRef& session, T& message) {
      session->Send(GameClientPacketHandler::MakeSendBuffer(message));
    }

    void HandleClientEnter(PacketSessionRef session, protocol::GameClientEnter packet);
  };
}
