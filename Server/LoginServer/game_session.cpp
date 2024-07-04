#include "pch.h"
#include "game_session.h"
#include "game_session_manager.h"
#include "login_client_packet_handler.h"
#include "room.h"


void GameSession::OnConnected() {
  GameSessionManager::GetInstance().Add(std::static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::OnDisconnected() {
  GameSessionManager::GetInstance().Remove(std::static_pointer_cast<GameSession>(shared_from_this()));

  if (_player) {
    if (const auto room = _room.lock()) {
      room->DoAsync(&Room::Leave, _player);
    }
  }

  _player = nullptr;
  _players.clear();

  std::cout << "GameSession Disconnected\n";
}

void GameSession::OnRecvPacket(std::byte* buffer, const int32_t len) {
  PacketSessionRef session = GetPacketSession();
  PacketHeader*    header = reinterpret_cast<PacketHeader*>(buffer);

  LoginClientPacketHandler::HandlePacket(session, buffer, len);
}

void GameSession::OnSend(const int32_t len) {}
