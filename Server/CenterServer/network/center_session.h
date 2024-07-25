#pragma once
#include "data/server_info.h"

#include "network/session.h"

namespace center {
  class CenterSession final : public PacketSession {
  public:
    CenterSession() : _session_id(_session_id_generator.fetch_add(1)) {
      std::cout << "CenterSession Constructor\n";
    }

    ~CenterSession() override {
      std::cout << "CenterSession Destructor\n";
    }

  public:
    void RegisterServer(const String& server_name, const String& server_ip, uint16_t server_port);

  public:
    bool IsRegistered() const { return _is_registered; }
    std::shared_ptr<common::ServerInfo> GetServerInfo() const { return _server_info; }
    int32_t GetSessionId() const { return _session_id; }

  protected:
    void OnConnected() override;
    void OnDisconnected() override;
    void OnRecvPacket(std::byte* buffer, const int32_t len) override;
    void OnSend(const int32_t len) override;

  private:
    std::shared_ptr<common::ServerInfo> _server_info = nullptr;
    bool _is_registered = false;
    int32_t _session_id = 0;

  private:
    inline static Atomic<int32_t> _session_id_generator = 0;
  };
}
