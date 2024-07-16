#pragma once
#include "network/protocol/center_protocol.pb.h"

#include "job/job_queue.h"

class CenterHandler final : public JobQueue {
public:
  void HandleServerRegisterRequest(PacketSessionRef session, protocol::CenterClientRegisterRequest request);

private:
  void SendResponse(const CenterSessionRef& session, const protocol::CenterServerRegisterResponse& response);
};
