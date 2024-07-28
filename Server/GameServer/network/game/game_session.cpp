#include "pch.h"
#include "game_session.h"

#include "game_client_packet_handler.h"
#include "game_session_manager.h"

#include "game/player_manager.h"


void GameSession::OnConnected() {
  GameSessionManager::GetInstance().Add(std::static_pointer_cast<GameSession>(shared_from_this()));
  std::cout << "GameSession Connected\n";
}

void GameSession::OnDisconnected() {
  if (_player_id != 0) {
    PlayerManager::GetInstance().RemovePlayer(_player_id);
  }

  GameSessionManager::GetInstance().Remove(std::static_pointer_cast<GameSession>(shared_from_this()));

  std::cout << "GameSession Disconnected\n";
}

void GameSession::OnRecvPacket(std::byte* buffer, const int32_t len) {
  PacketSessionRef session = GetPacketSession();
  PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

  GameClientPacketHandler::HandlePacket(session, buffer, len);
}

void GameSession::OnSend(const int32_t len) {}
