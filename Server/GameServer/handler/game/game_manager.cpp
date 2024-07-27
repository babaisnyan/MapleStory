#include "pch.h"
#include "game_manager.h"

void GameManager::HandleClientEnter(const PacketSessionRef& shared, const protocol::GameClientEnter& packet) const {
  _game_handler->DoAsync(&GameHandler::HandleClientEnter, shared, packet);
}
