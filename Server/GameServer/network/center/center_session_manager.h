#pragma once

namespace game {
  class CenterSessionManager {
  public:
    static CenterSessionManager& GetInstance() {
      static CenterSessionManager instance;
      return instance;
    }

    void SetCenterSession(const CenterSessionRef& center_session) { _center_session = center_session; }
    void Reset() { _center_session.reset(); }

    void Send(const SendBufferRef& send_buffer) const;

  private:
    CenterSessionRef _center_session;
  };
}
