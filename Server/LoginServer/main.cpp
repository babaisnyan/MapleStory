#include "pch.h"

#include "network/socket_utils.h"
#include "network/login/login_server.h"
#include "thread/thread_manager.h"


int main() {
  std::wcout.imbue(std::locale("kor"));

  LoginServer::GetInstance().Init();

  ThreadManager::GetInstance().Join();
  SocketUtils::Clear();
}
