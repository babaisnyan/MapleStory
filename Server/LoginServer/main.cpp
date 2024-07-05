#include "pch.h"

#include "login_server.h"

#include "network/socket_utils.h"

#include "thread/thread_manager.h"

std::unique_ptr<LoginServer> login_server;

int main() {
  std::wcout.imbue(std::locale("kor"));

  login_server = std::make_unique<LoginServer>();
  login_server->Init();

  ThreadManager::GetInstance().Join();
  SocketUtils::Clear();
}
