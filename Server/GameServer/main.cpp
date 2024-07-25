#include "pch.h"

#include <iostream>

#include "network/socket_utils.h"
#include "network/game/game_server.h"

#include "thread/thread_manager.h"

int main() {
  std::wcout.imbue(std::locale("kor"));

  GameServer::GetInstance().Init();

  ThreadManager::GetInstance().Join();
  SocketUtils::Clear();
}
