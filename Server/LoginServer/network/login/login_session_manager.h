#pragma once

namespace login {
  class LoginSessionManager {
  public:
    static LoginSessionManager& GetInstance() {
      static LoginSessionManager instance;
      return instance;
    }

  public:
    void Add(const LoginSessionRef& session);
    void Remove(const LoginSessionRef& session);

    void Broadcast(const SendBufferRef& send_buffer) const;

  private:
    tbb::concurrent_hash_map<int32_t, LoginSessionRef> _login_sessions;

    DECLARE_ACCESSOR_TYPE(_login_sessions);
    DECLARE_CONST_ACCESSOR_TYPE(_login_sessions);
  };
}
