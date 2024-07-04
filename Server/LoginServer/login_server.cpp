#include "pch.h"

#include <functional>

#include "game_session.h"
#include "game_session_manager.h"
#include "gen_procedures.h"
#include "login_client_packet_handler.h"
#include "database/db_connection_pool.h"
#include "database/db_synchronizer.h"
#include "memory/memory.h"
#include "network/iocp_core.h"
#include "network/service.h"
#include "network/socket_utils.h"
#include "thread/thread_manager.h"

enum {
  kWorkerTick = 64
};

void DoWorkerJob(const ServerServiceRef& service) {
  while (true) {
    LEndTickCount = GetTickCount64() + kWorkerTick;

    service->GetCore()->Dispatch(10);

    ThreadManager::DistributeReservedJobs();
    ThreadManager::FlushGlobalQueue();
  }
}

int main() {
  std::wcout.imbue(std::locale("kor"));

  ASSERT_CRASH(DbConnectionPool::GetInstance().Connect(1, L"DRIVER={SQL Server};SERVER=BABA\\SQLEXPRESS;DATABASE=ServerDb;Trusted_Connection=Yes"));
  DbConnection*  connection = DbConnectionPool::GetInstance().GetConnection();
  DbSynchronizer synchronizer(*connection);
  synchronizer.Synchronize(L"GameDB.xml");

  LoginClientPacketHandler::Init();
  SocketUtils::Init();
  atexit(SocketUtils::Clear);

  const ServerServiceRef service = MakeShared<ServerService>(NetworkAddress(L"127.0.0.1", 7777),
                                                             MakeShared<IocpCore>(),
                                                             MakeShared<GameSession>,
                                                             100);

  ASSERT_CRASH(service->Start());

  for (int i = 0; i < 5; i++) {
    ThreadManager::GetInstance().Launch([=] {
      while (true) {
        DoWorkerJob(service);
      }
    });
  }

  while (true) {
    protocol::LoginServerChat packet;
    packet.set_message("Hello");
    auto send_buffer = LoginClientPacketHandler::MakeSendBuffer(packet);

    GameSessionManager::GetInstance().Broadcast(send_buffer);
    std::this_thread::sleep_for(1s);
  }

  ThreadManager::GetInstance().Join();
}
