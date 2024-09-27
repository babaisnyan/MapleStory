#include "pch.h"
#include "stand_state.h"

#include "game/map/map_instance.h"
#include "game/objects/mob/monster.h"
#include "game/objects/player/player.h"

#include "utils/randomizer.h"

void StandState::Enter(const std::shared_ptr<Monster>& mob) {
  mob->OnStatusUpdated();
}

void StandState::Update(const std::shared_ptr<Monster>& mob, const float delta) {
  if (mob->GetTemplate()->CanBodyAttack()) {
    ProcessCollision(mob);
  }

  if (mob->GetTemplate()->HasAction(protocol::MOB_ACTION_TYPE_ATTACK) && mob->GetTemplate()->CanFirstAttack() && !mob->HasTarget()) {
    const auto target = FindNearestPlayer(mob);

    if (target) {
      mob->ChangeTarget(target);

      if (mob->IsAttackReady() && mob->IsTargetInAttackRange()) {
        mob->ChangeState(protocol::MOB_ACTION_TYPE_ATTACK);
      } else {
        mob->ChangeState(protocol::MOB_ACTION_TYPE_MOVE);
      }

      return;
    }
  }

  if (!mob->HasTarget()) {
    const auto random = utils::random::Rand(150);

    if (random == 50) {
      mob->SetFlip(utils::random::IsSuccess(50));
      mob->OnStatusUpdated();
    } else if (random == 100 || random == 101) {
      mob->SetFlip(random == 101);

      const auto min_x = mob->IsFlipped() ? mob->GetSpawnPoint()->GetMinX() : mob->GetX() + 10;
      const auto max_x = mob->IsFlipped() ? mob->GetX() - 10 : mob->GetSpawnPoint()->GetMaxX();
      const auto map_min_x = mob->GetSpawnPoint()->GetMinX();
      const auto map_max_x = mob->GetSpawnPoint()->GetMaxX();
      const auto random_x = utils::random::RandDouble(min_x, max_x);

      if (random_x > map_min_x && random_x < map_max_x) {
        mob->SetTargetPosition(random_x, mob->GetY());
        mob->ChangeState(protocol::MOB_ACTION_TYPE_MOVE);
      }
    }
  } else {
    if (mob->IsTargetInAttackRange()) {
      if (mob->IsAttackReady()) {
        mob->ChangeState(protocol::MOB_ACTION_TYPE_ATTACK);
      }
    } else {
      mob->ChangeState(protocol::MOB_ACTION_TYPE_MOVE);
    }
  }
}
