#include "pch.h"
#include "mob_state.h"

#include "game/map/map_instance.h"
#include "game/objects/game_object.h"
#include "game/objects/mob/monster.h"
#include "game/objects/player/player.h"

void MobState::ProcessCollision(const std::shared_ptr<Monster>& mob) {
  const auto now = GetTickCount64();
  const auto& mob_position = mob->GetPosition();
  const auto& [width , height] = mob->GetTemplate()->GetCollisionSize(protocol::MOB_ACTION_TYPE_STAND);
  const auto map = mob->GetMap().lock();
  const auto map_size = map->GetSize();
  const auto grid_x_radius = std::max(width / MsCoordinate::kGridSize / 2, 1);
  const auto grid_y_radius = std::max(height / MsCoordinate::kGridSize, 1);

  for (int y = std::max(mob_position.grid_y, static_cast<int16_t>(0)); y > mob_position.grid_y - grid_y_radius; --y) {
    for (int x = std::max(mob_position.grid_x - grid_x_radius, 0); x < mob_position.grid_x + grid_x_radius; ++x) {
      if (x < 0 || y < 0 || x >= map_size.first / MsCoordinate::kGridSize || y >= map_size.second / MsCoordinate::kGridSize) {
        continue;
      }

      const auto& objects = map->GetObjects(x, y);

      for (const auto& object : objects) {
        if (object->GetObjectType() != GameObject::ObjectType::kPlayer) {
          continue;
        }

        if (!object->IsAlive()) {
          continue;
        }

        const auto player = std::static_pointer_cast<Player>(object);
        const auto collision_width = static_cast<float>(width) / 2.0f;
        const auto collision_height = static_cast<float>(height) / 2.0f;
        const auto min_x = mob->GetX() - collision_width;
        const auto max_x = mob->GetX() + collision_width;

        if (player->GetX() - 16 > min_x && player->GetX() + 16 < max_x && player->GetY() > mob->GetY() - collision_height && player->GetY() < mob->GetY() + collision_height) {
          player->OnCollideMob(mob, now);
        }
      }
    }
  }
}

std::shared_ptr<Player> MobState::FindNearestPlayer(const std::shared_ptr<Monster>& mob) {
  const auto map = mob->GetMap().lock();

  if (!map) {
    return nullptr;
  }

  const auto map_size = map->GetGridSize();
  const auto& mob_position = mob->GetPosition();
  std::shared_ptr<Player> nearest_player = nullptr;

  for (auto x = 0; x < map_size.first; ++x) {
    const auto& objects = map->GetObjects(x, mob_position.grid_y);

    for (const auto& game_object : objects) {
      if (game_object->GetObjectType() != GameObject::ObjectType::kPlayer) {
        continue;
      }

      if (!game_object->IsAlive()) {
        continue;
      }

      if (!nearest_player) {
        nearest_player = std::static_pointer_cast<Player>(game_object);
        continue;
      }

      const auto nearest_distance = std::abs(nearest_player->GetX() - mob->GetX());
      const auto current_distance = std::abs(game_object->GetX() - mob->GetX());

      if (current_distance < nearest_distance) {
        nearest_player = std::static_pointer_cast<Player>(game_object);
      }
    }
  }

  return nearest_player;
}
