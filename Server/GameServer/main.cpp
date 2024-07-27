#include "pch.h"

#include <iostream>

#include "data/item_provider.h"

#include "network/socket_utils.h"
#include "network/game/game_server.h"

#include "thread/thread_manager.h"

int main() {
  std::wcout.imbue(std::locale("kor"));

  ThreadManager::GetInstance().Launch([] { ItemProvider::GetInstance().Init(); });

  ThreadManager::GetInstance().Join();

  GameServer::GetInstance().Init();
  SocketUtils::Clear();
}
