#pragma once

namespace login {
  class CenterServerSession;

  class CenterSessionManager {
  public:
    static CenterSessionManager& GetInstance() {
      static CenterSessionManager instance;
      return instance;
    }

    void SetCenterSession(const std::shared_ptr<CenterServerSession>& center_session) { _center_session = center_session; }
    void Reset() { _center_session.reset(); }

    void Send(const SendBufferRef& send_buffer) const;

  private:
    std::shared_ptr<CenterServerSession> _center_session;
  };
}
