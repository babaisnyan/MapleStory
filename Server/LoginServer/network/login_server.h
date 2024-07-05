#pragma once

class LoginServer {
  enum {
    kWorkerTick = 64
  };

public:
  LoginServer() = default;

  void Init();

private:
  void StartLoginServer();

public:
  ServerServiceRef GetLoginService() const { return _login_service; }

private:
  ServerServiceRef _login_service;
};
