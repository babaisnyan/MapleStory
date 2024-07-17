#pragma once
#include "login_session.h"

class LoginSessionManager {
public:
  static LoginSessionManager& GetInstance() {
    static LoginSessionManager instance;
    return instance;
  }

public:
  void Add(const LoginSessionRef& session);
  void Remove(const LoginSessionRef& session);

  void Broadcast(const SendBufferRef& send_buffer);

private:
  USE_LOCK;
  Set<LoginSessionRef> _login_sessions;
};
