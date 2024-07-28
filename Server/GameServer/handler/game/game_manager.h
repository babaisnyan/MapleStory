#pragma once
#include "game_handler.h"

namespace game {
  class GameManager {
  private:
    GameManager() : _game_handler(std::make_shared<GameHandler>()) {}
    ~GameManager() = default;

  public:
    static GameManager& GetInstance() {
      static GameManager instance;
      return instance;
    }

    void HandleClientEnter(const PacketSessionRef& shared, const protocol::GameClientEnter& packet) const;

  private:
    std::shared_ptr<GameHandler> _game_handler;
  };
}
