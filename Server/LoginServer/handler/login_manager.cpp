#include "pch.h"
#include "login_manager.h"

void LoginManager::HandleLoginRequest(const PacketSessionRef& session, const protocol::LoginClientLogin& request) {
  _login_handler->DoAsync(&LoginHandler::HandleLogin, session, request);
}

void LoginManager::HandleCharacterListRequest(const PacketSessionRef& session, const protocol::LoginClientRequestCharacterList& request) {
  _login_handler->DoAsync(&LoginHandler::HandleCharacterList, session, request);
}

void LoginManager::HandleSelectCharacterRequest(const PacketSessionRef& session, const protocol::LoginClientSelectCharacter& request) {
  _login_handler->DoAsync(&LoginHandler::HandleSelectCharacter, session, request);
}
