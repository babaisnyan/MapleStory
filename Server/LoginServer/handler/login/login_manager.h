#pragma once

#include "login_handler.h"

namespace login {
  class LoginManager {
  private:
    LoginManager() : _login_handler(std::make_shared<LoginHandler>()) {}
    ~LoginManager() = default;

  public:
    static LoginManager& GetInstance() {
      static LoginManager instance;
      return instance;
    }

    void HandleLoginRequest(const PacketSessionRef& session, const protocol::LoginClientLogin& request);
    void HandleCharacterListRequest(const PacketSessionRef& session);
    void HandleSelectCharacterRequest(const PacketSessionRef& session, const protocol::LoginClientSelectCharacter& request);
    void HandleDeleteCharacterRequest(const PacketSessionRef& session, const protocol::LoginClientDeleteCharacter& request);
    void HandleCreateCharacterRequest(const PacketSessionRef& session, const protocol::LoginClientCreateCharacter& request);

  private:
    std::shared_ptr<LoginHandler> _login_handler;
  };
}
