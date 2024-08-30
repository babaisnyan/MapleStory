#include "pch.h"

#include <iostream>

#include "data/item_provider.h"
#include "data/map_provider.h"
#include "data/mob_provider.h"
#include "data/server_config.h"

#include "game/map/map_manager.h"

#include "network/socket_utils.h"
#include "network/game/game_server.h"

#include "thread/thread_manager.h"

int _tmain(const int argc, TCHAR* argv[]) {
  std::wcout.imbue(std::locale("kor"));

  if (argc < 2) {
    std::wcout << L"서버 설정 파일을 지정해주세요\n";
    return 0;
  }

  ServerConfig::GetInstance().Init(argv[1]);

  ThreadManager::GetInstance().Launch([] {
    ItemProvider::GetInstance().Init();
  });
  ThreadManager::GetInstance().Launch([] {
    MobProvider::GetInstance().Init();
    MapProvider::GetInstance().Init();
    MapManager::GetInstance().LoadMaps();
  });

  ThreadManager::GetInstance().Join();

  GameServer::GetInstance().Init();
  ThreadManager::GetInstance().Join();

  SocketUtils::Clear();
}
