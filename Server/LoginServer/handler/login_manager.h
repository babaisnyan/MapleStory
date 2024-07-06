#pragma once
#include "login_handler.h"

#include "memory/memory.h"

class LoginManager {
private:
  LoginManager() : _login_handler(MakeShared<LoginHandler>()) {}
  ~LoginManager() = default;

public:
  static LoginManager& GetInstance() {
    static LoginManager instance;
    return instance;
  }

  void HandleLoginRequest(const PacketSessionRef& session, const protocol::LoginClientLogin& request);
  void HandleCharacterListRequest(const PacketSessionRef& session, const protocol::LoginClientRequestCharacterList& request);

private:
  std::shared_ptr<LoginHandler> _login_handler;
};
