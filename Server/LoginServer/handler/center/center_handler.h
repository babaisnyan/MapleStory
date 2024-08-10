#pragma once

#include "job/job_queue.h"
#include "network/center/center_server_packet_handler.h"
#include "network/center/center_server_session.h"


namespace login {
  class CenterHandler final : public JobQueue {
  public:
    void HandleMigrationResponse(const PacketSessionRef& session, const protocol::CenterServerMigrationResponse& packet);
    void HandleMigrationRequest(const PacketSessionRef& session, const protocol::CenterServerMigrationRequest& packet);

    template <typename T> requires std::is_base_of_v<google::protobuf::Message, T>
    void SendPacket(const CenterSessionRef& session, T& message) {
      session->Send(CenterServerPacketHandler::MakeSendBuffer(message));
    }
  };
}
