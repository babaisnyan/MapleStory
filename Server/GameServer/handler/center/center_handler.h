#pragma once

#include "job/job_queue.h"

#include "network/center/center_server_packet_handler.h"
#include "network/center/center_server_session.h"
#include "network/protocol/center_protocol.pb.h"


namespace game {
  class CenterHandler final : public JobQueue {
  public:
    void HandleMigrationResponse( PacketSessionRef session,  protocol::CenterServerMigrationResponse packet);
    void HandleMigrationRequest(const PacketSessionRef& session, const protocol::CenterServerMigrationRequest& packet);

  public:
    template <typename T> requires std::is_base_of_v<google::protobuf::Message, T>
    void SendPacket(const CenterSessionRef& session, T& message) {
      session->Send(CenterServerPacketHandler::MakeSendBuffer(message));
    }
  };
}
