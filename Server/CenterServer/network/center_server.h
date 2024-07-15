#pragma once

class CenterServer {
  enum {
    kWorkerTick = 64,
    kMaxDbConnection = 5,
  };

public:
  CenterServer() = default;

  void Init();

private:
  void StartCenterServer();

public:
  ServerServiceRef GetCenterService() const { return _center_service; }

private:
  ServerServiceRef _center_service;
};
