#pragma once
#include "network/session.h"

class LoginSession : public PacketSession {
public:
  ~LoginSession() override {
    std::cout << "LoginSession Destructor\n";
  }

public:


protected:
  void OnConnected() override;
  void OnDisconnected() override;
  void OnRecvPacket(std::byte* buffer, const int32_t len) override;
  void OnSend(const int32_t len) override;

private:
 
};
