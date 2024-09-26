#include "pch.h"
#include "hit_state.h"

#include "game/objects/mob/monster.h"

void HitState::Enter(const std::shared_ptr<Monster>& mob) {}

void HitState::Update(const std::shared_ptr<Monster>& mob, const float delta) {
  if (mob->GetTemplate()->CanBodyAttack()) {
    ProcessCollision(mob);
  }

  if (mob->GetAnimationTime() >= mob->GetTemplate()->GetActionLength(protocol::MOB_ACTION_TYPE_HIT)) {
    mob->ChangeState(protocol::MOB_ACTION_TYPE_STAND);
  }
}
