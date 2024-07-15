#include "pch.h"
#include "center_server.h"

#include "center_client_packet_handler.h"
#include "center_session.h"

#include "database/db_connection_pool.h"

#include "memory/memory.h"

#include "network/service.h"
#include "network/socket_utils.h"

#include "thread/thread_manager.h"

void CenterServer::Init() {
  SocketUtils::Init();
  CenterClientPacketHandler::Init();

  ASSERT_CRASH(DbConnectionPool::GetInstance().Connect(kMaxDbConnection,
                 L"DRIVER={SQL Server};SERVER=BABA\\SQLEXPRESS;DATABASE=Maplestory;Trusted_Connection=Yes"));

  StartCenterServer();
}

void CenterServer::StartCenterServer() {
  _center_service = MakeShared<ServerService>(NetworkAddress(L"127.0.0.1", 10000),
                                             MakeShared<IocpCore>(),
                                             MakeShared<CenterSession>,
                                             5);

  ASSERT_CRASH(_center_service->Start());

  for (int i = 0; i < 5; i++) {
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
