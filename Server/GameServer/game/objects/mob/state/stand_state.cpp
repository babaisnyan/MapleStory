#include "pch.h"
#include "stand_state.h"

#include "game/objects/mob/monster.h"

#include "utils/randomizer.h"

void StandState::Enter(const std::shared_ptr<Monster>& mob) {
  mob->ResetAnimationTime();
}

void StandState::Update(const std::shared_ptr<Monster>& mob, const float delta) {
  mob->AddAnimationTime(delta);

  if (!mob->IsTargetInDistance()) {
    mob->ResetTarget();
  }

  if (mob->GetTemplate()->CanFirstAttack() && !mob->HasTarget()) {
    const auto target = FindNearestPlayer(mob);

    if (target) {
      mob->ChangeTarget(target);
    }
  }
}

void StandState::PostUpdate(const std::shared_ptr<Monster>& mob) {
  if (mob->HasTarget()) {
    if (mob->IsAttackReady() && mob->IsTargetInDistance()) {
      mob->ChangeState(protocol::MOB_ACTION_TYPE_ATTACK);
    } else {
      mob->ChangeState(protocol::MOB_ACTION_TYPE_MOVE);
    }
  } else {
    switch (utils::random::Rand(150)) {
    case 50:
      mob->SetFlip(false);
      mob->ChangeState(protocol::MOB_ACTION_TYPE_MOVE);
      break;
    case 100:
      mob->SetFlip(true);
      mob->ChangeState(protocol::MOB_ACTION_TYPE_MOVE);
      break;
    }
  }
}
