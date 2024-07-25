#pragma once

#include "network/session.h"

namespace game {
  class GameSession final : public PacketSession {
  public:
    GameSession() : _session_id(_session_id_generator.fetch_add(1)) {
      std::cout << "GameSession Constructor\n";
    }

    ~GameSession() override {
      std::cout << "GameSession Destructor\n";
    }

  public:
    int32_t GetSessionId() const { return _session_id; }

  protected:
    void OnConnected() override;
    void OnDisconnected() override;
    void OnRecvPacket(std::byte* buffer, const int32_t len) override;
    void OnSend(const int32_t len) override;

  private:
    int32_t _session_id = 0;

  private:
    inline static Atomic<int32_t> _session_id_generator = 0;
  };

  using GameSessionRef = std::shared_ptr<GameSession>;
}
