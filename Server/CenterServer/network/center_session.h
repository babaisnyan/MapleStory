#pragma once
#include "network/session.h"

class CenterSession final : public PacketSession {
public:
  ~CenterSession() override {
    std::cout << "CenterSession Destructor\n";
  }

public:
  void RegisterServer(const String& server_name, const String& server_ip, uint16_t server_port);

public:
  bool IsRegistered() const { return _is_registered; }
  StringView GetServerName() const { return _server_name; }
  StringView GetServerIp() const { return _server_ip; }
  uint16_t GetServerPort() const { return _server_port; }

protected:
  void OnConnected() override;
  void OnDisconnected() override;
  void OnRecvPacket(std::byte* buffer, const int32_t len) override;
  void OnSend(const int32_t len) override;

private:
  String _server_name;
  String _server_ip;
  uint16_t _server_port = 0;
  bool _is_registered = false;
};
