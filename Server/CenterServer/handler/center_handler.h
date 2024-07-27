#pragma once
#include "network/protocol/center_protocol.pb.h"

#include "job/job_queue.h"

#include "network/center_client_packet_handler.h"
#include "network/center_session.h"

namespace center {
  class CenterHandler final : public JobQueue {
  public:
    void HandleServerRegisterRequest(PacketSessionRef session,  protocol::CenterClientRegisterRequest request);
    void HandleServerMigrationRequest( PacketSessionRef session,  protocol::CenterClientMigrationRequest request);
    void HandleServerMigrationResponse( PacketSessionRef session,  protocol::CenterClientMigrationResponse response);

  private:
    template <typename T> requires std::is_base_of_v<google::protobuf::Message, T>
    void SendResponse(const CenterSessionRef& session, const T& response) {
      const SendBufferRef send_buffer = CenterClientPacketHandler::MakeSendBuffer(response);
      session->Send(send_buffer);
    }
  };
}
