#pragma once

namespace game {
  class GameServer {
    enum {
      kWorkerTick      = 64,
      kMaxDbConnection = 15,
    };

  public:
    static GameServer& GetInstance() {
      static GameServer instance;
      return instance;
    }

    void Init();
    void StartGameServer();

  private:
    void ConnectCenterServer();

  public:
    ServerServiceRef GetGameService() const { return _game_service; }
    ClientServiceRef GetCenterService() const { return _center_service; }

  private:
    ServerServiceRef _game_service;
    ClientServiceRef _center_service;
  };
}
