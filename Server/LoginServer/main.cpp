#include "pch.h"

#include "data/server_config.h"

#include "network/socket_utils.h"
#include "network/login/login_server.h"
#include "thread/thread_manager.h"

int _tmain(const int argc, TCHAR* argv[]) {
  std::wcout.imbue(std::locale("kor"));

  if (argc < 2) {
    std::wcout << L"서버 설정 파일을 지정해주세요\n";
    return 0;
  }

  ServerConfig::GetInstance().Init(argv[1]);
  LoginServer::GetInstance().Init();
  ThreadManager::GetInstance().Join();

  SocketUtils::Clear();
}
