#include "pch.h"
#include "object_move_queue.h"

#include "game/map/map_instance.h"
#include "game/map/map_manager.h"
#include "game/objects/player/player.h"

#include "network/game/game_session.h"

void ObjectMoveQueue::MovePlayer(const PacketSessionRef& session, const protocol::GameClientPlayerMove& packet) {
  const auto game_session = std::static_pointer_cast<GameSession>(session);
  const auto player = game_session->GetPlayer();
  const auto map = MapManager::GetInstance().GetMapInstance(game_session->GetPlayer()->GetMap());

  if (!map.has_value()) {
    return;
  }

  player->UpdatePosition(packet.x(), packet.y());

  protocol::GameServerPlayerMove response;
  response.set_object_id(player->GetObjectId());
  response.set_x(packet.x());
  response.set_y(packet.y());
  response.set_is_right(packet.is_right());
  response.set_animation(packet.animation());

  map.value()->BroadCast(response, player->GetId());
}
