#include "pch.h"
#include "login_manager.h"

void LoginManager::HandleLoginRequest(const PacketSessionRef& session, const protocol::LoginClientLogin& request) {
  _login_handler->DoAsync(&LoginHandler::HandleLogin, session, request);
}
