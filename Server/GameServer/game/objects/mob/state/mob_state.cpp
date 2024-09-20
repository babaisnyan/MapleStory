#include "pch.h"
#include "mob_state.h"

#include "game/map/map_instance.h"
#include "game/objects/game_object.h"
#include "game/objects/mob/monster.h"
#include "game/objects/player/player.h"

void MobState::ProcessCollision(const std::shared_ptr<Monster>& mob) {
  const auto now = GetTickCount64();
  const auto& mob_position = mob->GetPosition();
  const auto& [width, height] = mob->GetTemplate()->GetCollisionSize(mob->GetCurrentState());
  const auto map = mob->GetMap().lock();
  const auto grid_x_radius = std::max(width / MsCoordinate::kGridSize / 2, 1);
  const auto grid_y_radius = std::max(height / MsCoordinate::kGridSize, 1);

  for (int y = std::max(mob_position.grid_y, static_cast<int16_t>(0)); y > mob_position.grid_y - grid_y_radius; --y) {
    for (int x = std::max(mob_position.grid_x - grid_x_radius, 0); x < mob_position.grid_x + grid_x_radius; ++x) {
      if (x < 0 || y < 0) {
        continue;
      }

      const auto& objects = map->GetObjects(x, y);

      for (const auto& object : objects) {
        if (object->GetObjectType() != GameObject::ObjectType::kPlayer) {
          continue;
        }

        const auto player = std::static_pointer_cast<Player>(object);
        const auto collision_width = width / 2.0f;
        const auto collision_height = height / 2.0f;

        if (object->GetX() > mob->GetX() - collision_width && object->GetX() < mob->GetX() + collision_width &&
            object->GetY() > mob->GetY() - collision_height && object->GetY() < mob->GetY() + collision_height) {
          player->OnCollideMob(mob, now);
        }
      }
    }
  }
}

std::shared_ptr<Player> MobState::FindNearestPlayer(const std::shared_ptr<Monster>& mob) {
  return nullptr;
}
