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

  private:
    std::shared_ptr<GameHandler> _game_handler;
  };
}
