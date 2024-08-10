#include "pch.h"
#include "login_manager.h"

void LoginManager::HandleLoginRequest(const PacketSessionRef& session, const protocol::LoginClientLogin& request) {
  _login_handler->DoAsync(&LoginHandler::HandleLogin, session, request);
}

void LoginManager::HandleCharacterListRequest(const PacketSessionRef& session) {
  _login_handler->DoAsync(&LoginHandler::HandleCharacterList, session);
}

void LoginManager::HandleSelectCharacterRequest(const PacketSessionRef& session, const protocol::LoginClientSelectCharacter& request) {
  _login_handler->DoAsync(&LoginHandler::HandleSelectCharacter, session, request);
}

void LoginManager::HandleDeleteCharacterRequest(const PacketSessionRef& session, const protocol::LoginClientDeleteCharacter& request) {
  _login_handler->DoAsync(&LoginHandler::HandleDeleteCharacter, session, request);
}

void LoginManager::HandleCreateCharacterRequest(const PacketSessionRef& session, const protocol::LoginClientCreateCharacter& request) {
  _login_handler->DoAsync(&LoginHandler::HandleCreateCharacter, session, request);
}
