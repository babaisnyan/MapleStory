#pragma once

#include "network/session.h"

namespace game {
  class Player;

  class GameSession final : public PacketSession {
  public:
    GameSession() : _session_id(_session_id_generator.fetch_add(1)) {}
    ~GameSession() override = default;

  public:
    int32_t GetSessionId() const;

    std::shared_ptr<Player> GetPlayer();
    void SetPlayer(const std::shared_ptr<Player>& player);

  protected:
    void OnConnected() override;
    void OnDisconnected() override;
    void OnRecvPacket(std::byte* buffer, const int32_t len) override;
    void OnSend(const int32_t len) override;

  private:
    int32_t _session_id = 0;
    int32_t _player_id = 0;
    std::shared_ptr<Player> _player = nullptr;

  private:
    inline static Atomic<int32_t> _session_id_generator = 0;
  };

  using GameSessionRef = std::shared_ptr<GameSession>;
}
