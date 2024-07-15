#include "pch.h"

#include "network/center_server.h"

#include "network/socket_utils.h"

#include "thread/thread_manager.h"

std::unique_ptr<CenterServer> center_server;

int main() {
  std::wcout.imbue(std::locale("kor"));

  center_server = std::make_unique<CenterServer>();
  center_server->Init();

  ThreadManager::GetInstance().Join();
  SocketUtils::Clear();
}
