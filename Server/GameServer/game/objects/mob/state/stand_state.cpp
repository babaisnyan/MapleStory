#include "pch.h"
#include "stand_state.h"

#include "game/map/map_instance.h"
#include "game/objects/mob/monster.h"

#include "network/protocol/game_protocol.pb.h"

#include "utils/randomizer.h"

void StandState::Enter(const std::shared_ptr<Monster>& mob) {
  mob->ResetAnimationTime();

  protocol::GameServerMobMove move;
  move.set_object_id(mob->GetObjectId());
  move.set_x(mob->GetX());
  move.set_y(mob->GetY());
  move.set_flip(mob->IsFlipped());
  move.set_state(mob->GetCurrentState());

  mob->GetMap().lock()->BroadCast(move, nullptr);
}

void StandState::Update(const std::shared_ptr<Monster>& mob, const float delta) {
  mob->AddAnimationTime(delta);

  const auto mob_position = mob->GetPosition();
  const auto collision = mob->GetTemplate()->GetCollisionSize(mob->GetCurrentState());
  const auto map = mob->GetMap().lock();
  const auto grid_x_radius = std::max(collision.first / MsCoordinate::kGridSize / 2, 1);
  const auto grid_y_radius = std::max(collision.second / MsCoordinate::kGridSize, 1);

  for (int y = mob_position.grid_y - grid_y_radius; mob_position.grid_y > 0 && y <= mob_position.grid_y + grid_y_radius; ++y) {
    for (int x = mob_position.grid_x - grid_x_radius; mob_position.grid_x > 0 && x <= mob_position.grid_x + grid_x_radius; ++x) {
      const auto objects = map->GetObjects(x, y);

      for (const auto& object : objects) {
        if (static_cast<GameObject::ObjectType>(object->GetObjectId() / GameObject::kObjectRange) != GameObject::ObjectType::kPlayer) {
          continue;
        }

        const auto collision_width = collision.first / 2.0f;
        const auto collision_height = collision.second / 2.0f;

        if (object->GetX() > mob->GetX() - collision_width && object->GetX() < mob->GetX() + collision_width &&
            object->GetY() > mob->GetY() - collision_height && object->GetY() < mob->GetY() + collision_height) {
          std::cout << "Player is in collision area\n";
        }
      }
    }
  }


  if (!mob->IsTargetInDistance()) {
    mob->ResetTarget();
  }

  if (mob->GetTemplate()->CanFirstAttack() && !mob->HasTarget()) {
    const auto target = FindNearestPlayer(mob);

    if (target) {
      mob->ChangeTarget(target);

      if (mob->IsAttackReady() && mob->IsTargetInDistance()) {
        mob->ChangeState(protocol::MOB_ACTION_TYPE_ATTACK);
      } else {
        mob->ChangeState(protocol::MOB_ACTION_TYPE_MOVE);
      }
      return;
    }
  }

  const auto random = utils::random::Rand(150);

  if (random == 50) {
    mob->SetFlip(utils::random::IsSuccess(50));

    protocol::GameServerMobMove move;
    move.set_object_id(mob->GetObjectId());
    move.set_x(mob->GetX());
    move.set_y(mob->GetY());
    move.set_flip(mob->IsFlipped());
    move.set_state(mob->GetCurrentState());

    mob->GetMap().lock()->BroadCast(move, nullptr);
  } else if (random == 100 || random == 101) {
    mob->SetFlip(random == 101);

    const auto min_x = mob->IsFlipped() ? mob->GetSpawnPoint()->GetMinX() : mob->GetX() + 10;
    const auto max_x = mob->IsFlipped() ? mob->GetX() - 10 : mob->GetSpawnPoint()->GetMaxX();
    const auto map_min_x = mob->GetSpawnPoint()->GetMinX();
    const auto map_max_x = mob->GetSpawnPoint()->GetMaxX();
    const auto random_x = utils::random::RandFloat(min_x, max_x);

    if (random_x > map_min_x && random_x < map_max_x) {
      mob->SetTargetPosition(random_x, mob->GetY());
      mob->ChangeState(protocol::MOB_ACTION_TYPE_MOVE);
    }
  }
}
