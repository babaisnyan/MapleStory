#pragma once

class LoginServer {
  enum {
    kWorkerTick      = 64,
    kMaxDbConnection = 15,
  };

public:
  static LoginServer& GetInstance() {
    static LoginServer instance;
    return instance;
  }

  void Init();
  void StartLoginServer();

private:
  void ConnectCenterServer();

public:
  ServerServiceRef GetLoginService() const { return _login_service; }
  ClientServiceRef GetCenterService() const { return _center_service; }

private:
  ServerServiceRef _login_service;
  ClientServiceRef _center_service;
};
