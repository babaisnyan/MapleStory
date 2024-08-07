#include "pch.h"
#include "game_session.h"

#include "game_client_packet_handler.h"
#include "game_session_manager.h"

#include "game/player_manager.h"
#include "game/map/map_instance.h"
#include "game/map/map_manager.h"
#include "game/objects/player/player.h"


int32_t GameSession::GetSessionId() const { return _session_id; }

std::shared_ptr<Player> GameSession::GetPlayer() {
  return _player;
}

void GameSession::SetPlayer(const std::shared_ptr<Player>& player) {
  _player = player;
  _player_id = player->GetId();
}

void GameSession::OnConnected() {
  GameSessionManager::GetInstance().Add(std::static_pointer_cast<GameSession>(shared_from_this()));
  std::cout << "GameSession Connected\n";
}

void GameSession::OnDisconnected() {
  if (_player_id != 0) {
    PlayerManager::GetInstance().RemovePlayer(_player_id);
  }

  GameSessionManager::GetInstance().Remove(std::static_pointer_cast<GameSession>(shared_from_this()));

  const auto map = MapManager::GetInstance().GetMapInstance(_player->GetMap());
  if (map.has_value()) {
    map.value()->RemovePlayer(_player_id);
  }

  std::cout << "GameSession Disconnected\n";
}

void GameSession::OnRecvPacket(std::byte* buffer, const int32_t len) {
  PacketSessionRef session = GetPacketSession();
  PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

  GameClientPacketHandler::HandlePacket(session, buffer, len);
}

void GameSession::OnSend(const int32_t len) {}
