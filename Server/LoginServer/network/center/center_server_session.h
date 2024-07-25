#pragma once

namespace login {
  class CenterServerSession final : public PacketSession {

  protected:
    void OnConnected() override;
    void OnDisconnected() override;
    void OnRecvPacket(std::byte* buffer, const int32_t len) override;
    void OnSend(const int32_t len) override;
  };

  using CenterServerSessionRef = std::shared_ptr<CenterServerSession>;
}
