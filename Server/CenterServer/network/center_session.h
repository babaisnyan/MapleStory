#pragma once
#include "network/session.h"

#include "protocol/center_struct.pb.h"

class CenterSession : public PacketSession {
public:
  ~CenterSession() override {
    std::cout << "CenterSession Destructor\n";
  }

protected:
  void OnConnected() override;
  void OnDisconnected() override;
  void OnRecvPacket(std::byte* buffer, const int32_t len) override;
  void OnSend(const int32_t len) override;

private:

};
