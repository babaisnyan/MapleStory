#include "pch.h"
#include "game_server.h"

#include "game_client_packet_handler.h"
#include "game_session.h"

#include "data/server_config.h"

#include "database/db_connection_pool.h"

#include "memory/memory.h"

#include "network/service.h"
#include "network/socket_utils.h"
#include "network/center/center_server_packet_handler.h"
#include "network/center/center_server_session.h"

#include "thread/thread_manager.h"

void GameServer::Init() {
  SocketUtils::Init();
  GameClientPacketHandler::Init();
  CenterServerPacketHandler::Init();

  ASSERT_CRASH(DbConnectionPool::GetInstance().Connect(kMaxDbConnection,
                 L"DRIVER={SQL Server};SERVER=BABA\\SQLEXPRESS;DATABASE=Maplestory;Trusted_Connection=Yes"));

  ConnectCenterServer();
}

void GameServer::StartGameServer() {
  const auto ip = ServerConfig::GetInstance().GetServerIp();
  const auto port = ServerConfig::GetInstance().GetServerPort();
  _game_service = MakeShared<ServerService>(NetworkAddress(ip, port),
                                            MakeShared<IocpCore>(),
                                            MakeShared<GameSession>,
                                            300);

  ASSERT_CRASH(_game_service->Start());

  for (int i = 0; i < 10; i++) {
    ThreadManager::GetInstance().Launch([this] {
      while (true) {
        LEndTickCount = GetTickCount64() + kWorkerTick;

        _game_service->GetCore()->Dispatch(10);

        ThreadManager::DistributeReservedJobs();
        ThreadManager::FlushGlobalQueue();
      }
    });
  }
}

void GameServer::ConnectCenterServer() {
  const auto ip = ServerConfig::GetInstance().GetCenterIp();
  const auto port = ServerConfig::GetInstance().GetCenterPort();
  _center_service = MakeShared<ClientService>(NetworkAddress(ip, port),
                                              MakeShared<IocpCore>(),
                                              MakeShared<CenterServerSession>,
                                              1);

  ASSERT_CRASH(_center_service->Start());

  for (int i = 0; i < 2; i++) {
    ThreadManager::GetInstance().Launch([this] {
      while (true) {
        LEndTickCount = GetTickCount64() + kWorkerTick;

        _center_service->GetCore()->Dispatch(10);

        ThreadManager::DistributeReservedJobs();
        ThreadManager::FlushGlobalQueue();
      }
    });
  }
}
