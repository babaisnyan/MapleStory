#include "pch.h"
#include "login_client_packet_handler.h"

#include "handler/login/login_manager.h"

namespace login {
  bool HandleLoginInvalid(PacketSessionRef& session, std::byte* buffer, const int32_t len) {
    return false;
  }

  bool HandleLoginClientLogin(const PacketSessionRef& session, const protocol::LoginClientLogin& packet) {
    LoginManager::GetInstance().HandleLoginRequest(session, packet);
    return true;
  }

  bool HandleLoginClientRequestCharacterList(const PacketSessionRef& session, const protocol::LoginClientRequestCharacterList& packet) {
    LoginManager::GetInstance().HandleCharacterListRequest(session);
    return true;
  }

  bool HandleLoginClientSelectCharacter(const PacketSessionRef& session, const protocol::LoginClientSelectCharacter& packet) {
    LoginManager::GetInstance().HandleSelectCharacterRequest(session, packet);
    return true;
  }

  bool HandleLoginClientDeleteCharacter(const PacketSessionRef& session, const protocol::LoginClientDeleteCharacter& packet) {
    LoginManager::GetInstance().HandleDeleteCharacterRequest(session, packet);
    return true;
  }

  bool HandleLoginClientCreateCharacter(const PacketSessionRef& session, const protocol::LoginClientCreateCharacter& packet) {
    LoginManager::GetInstance().HandleCreateCharacterRequest(session, packet);
    return true;
  }
}
