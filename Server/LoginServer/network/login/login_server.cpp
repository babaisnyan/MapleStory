﻿#include "pch.h"
#include "login_server.h"

#include "login_client_packet_handler.h"
#include "login_session.h"

#include "database/db_connection_pool.h"

#include "memory/memory.h"

#include "network/service.h"
#include "network/socket_utils.h"
#include "network/center/center_server_packet_handler.h"
#include "network/center/center_server_session.h"

#include "thread/thread_manager.h"

void LoginServer::Init() {
  SocketUtils::Init();
  LoginClientPacketHandler::Init();
  CenterServerPacketHandler::Init();

  ASSERT_CRASH(DbConnectionPool::GetInstance().Connect(kMaxDbConnection,
                 L"DRIVER={SQL Server};SERVER=BABA\\SQLEXPRESS;DATABASE=Maplestory;Trusted_Connection=Yes"));

  ConnectCenterServer();
}

void LoginServer::StartLoginServer() {
  _login_service = MakeShared<ServerService>(NetworkAddress(L"127.0.0.1", 7777),
                                             MakeShared<IocpCore>(),
                                             MakeShared<LoginSession>,
                                             100);

  std::cout << "Login Server Start" << std::endl;

  ASSERT_CRASH(_login_service->Start());

  for (int i = 0; i < 5; i++) {
    ThreadManager::GetInstance().Launch([this] {
      while (true) {
        LEndTickCount = GetTickCount64() + kWorkerTick;

        _login_service->GetCore()->Dispatch(10);

        ThreadManager::DistributeReservedJobs();
        ThreadManager::FlushGlobalQueue();
      }
    });
  }
}

void LoginServer::ConnectCenterServer() {
  _center_service = MakeShared<ClientService>(NetworkAddress(L"127.0.0.1", 10000),
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
