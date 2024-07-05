#pragma once
#include "../network/protocol/login_protocol.pb.h"

#include "job/job_queue.h"

class LoginHandler final : public JobQueue {
public:
  void HandleLogin(PacketSessionRef session, protocol::LoginClientLogin request);
};
