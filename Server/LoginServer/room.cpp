#include "pch.h"
#include "room.h"

#include <ranges>

#include "game_session.h"
#include "player.h"

std::shared_ptr<Room> GRoom = std::make_shared<Room>();

void Room::Enter(PlayerRef player) {
  _players[player->GetId()] = std::move(player);
}

void Room::Leave(PlayerRef player) {
  _players.erase(player->GetId());
}

void Room::Broadcast(SendBufferRef send_buffer) {
  for (const auto& val : _players | std::views::values) {
    val->GetSession()->Send(send_buffer);
  }
}