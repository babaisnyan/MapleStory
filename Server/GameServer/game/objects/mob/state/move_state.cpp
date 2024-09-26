#include "pch.h"
#include "move_state.h"

#include "game/map/map_instance.h"
#include "game/objects/mob/monster.h"
#include "game/objects/player/player.h"

#include "utils/randomizer.h"

void MoveState::Enter(const std::shared_ptr<Monster>& mob) {
  if (mob->HasTargetPosition()) {
    protocol::GameServerMobMove move;
    move.set_object_id(mob->GetObjectId());
    move.set_x(mob->GetX());
    move.set_y(mob->GetY());
    move.set_flip(mob->IsFlipped());
    move.set_state(mob->GetCurrentState());

    const auto target_position = mob->GetTargetPosition();

    if (target_position) {
      move.set_target_x(target_position->x);
      move.set_target_y(target_position->y);
    }

    mob->GetMap().lock()->BroadCast(move, nullptr);
  }
}

void MoveState::Update(const std::shared_ptr<Monster>& mob, const float delta) {
  if (mob->GetTemplate()->CanBodyAttack()) {
    ProcessCollision(mob);
  }

  if (mob->HasTarget()) {
    if (mob->IsTargetInAttackRange()) {
      mob->ChangeState(protocol::MOB_ACTION_TYPE_ATTACK);
      return;
    }

    const auto target = mob->GetTarget().lock();

    if (target) {
      mob->SetFlip(mob->GetX() > target->GetX());
      mob->SetTargetPosition(target->GetX(), mob->GetY());
    }
  } else {
    // TODO: 캐릭터 타겟 탐색
  }

  if (!mob->HasTargetPosition()) {
    mob->ChangeState(protocol::MOB_ACTION_TYPE_STAND);
    return;
  }

  if (!mob->HasTarget()) {
    const auto random = utils::random::Rand(150);

    if (random == 50) {
      mob->SetFlip(utils::random::IsSuccess(50));

      const auto min_x = mob->IsFlipped() ? mob->GetSpawnPoint()->GetMinX() : mob->GetX() + 10;
      const auto max_x = mob->IsFlipped() ? mob->GetX() - 10 : mob->GetSpawnPoint()->GetMaxX();
      const auto map_min_x = mob->GetSpawnPoint()->GetMinX();
      const auto map_max_x = mob->GetSpawnPoint()->GetMaxX();
      const auto random_x = utils::random::RandDouble(min_x, max_x);

      if (random_x > map_min_x && random_x < map_max_x) {
        mob->SetTargetPosition(random_x, mob->GetY());
        mob->ChangeState(protocol::MOB_ACTION_TYPE_MOVE);
        return;
      }
    } else if (random == 149) {
      mob->ChangeState(protocol::MOB_ACTION_TYPE_STAND);
      return;
    }
  }

  const auto dir = mob->IsFlipped() ? -1.0f : 1.0f;
  const auto x = mob->GetX() + dir * mob->GetSpeed() * delta;
  const auto y = mob->GetY();
  const auto min_x = mob->GetSpawnPoint()->GetMinX();
  const auto max_x = mob->GetSpawnPoint()->GetMaxX();
  const auto target_position = mob->GetTargetPosition();

  if (x > min_x && x < max_x) {
    const auto& position = mob->GetPosition();
    const auto old_x = position.grid_x;
    const auto old_y = position.grid_y;
    mob->UpdatePosition(x, y, mob->IsFlipped());

    if (position.grid_x != old_x || position.grid_y != old_y) {
      mob->GetMap().lock()->MoveObject(mob, old_x, old_y);
    }

    // std::cout << std::format("Monster {} move to x: {}, y: {}, speed: {}\n", mob->GetId(), x, y, mob->GetSpeed());

    if (std::abs(x - target_position->x) < 1.0f) {
      mob->ResetTargetPosition();
      mob->ChangeState(protocol::MOB_ACTION_TYPE_STAND);
    }
  } else {
    mob->ResetTargetPosition();
    mob->ChangeState(protocol::MOB_ACTION_TYPE_STAND);
  }
}
