#pragma once
#include "network/session.h"

class LoginSession : public PacketSession {
public:
  ~LoginSession() override {
    std::cout << "LoginSession Destructor\n";
  }

public:
  int32_t GetAccountId() const { return _account_id; }
  void    SetAccountId(const int32_t account_id) { _account_id = account_id; }

protected:
  void OnConnected() override;
  void OnDisconnected() override;
  void OnRecvPacket(std::byte* buffer, const int32_t len) override;
  void OnSend(const int32_t len) override;

private:
  int32_t _account_id = 0;
};
