#include "pch.h"
#include "hit_state.h"

#include "game/objects/mob/monster.h"

void HitState::Enter(const std::shared_ptr<Monster>& mob) {
  mob->ResetAnimationTime();
}

void HitState::Update(const std::shared_ptr<Monster>& mob, const float delta) {
  mob->AddAnimationTime(delta);
}

void HitState::PostUpdate(const std::shared_ptr<Monster>& mob) {
  if (mob->GetAnimationTime() >= mob->GetTemplate()->GetActionLength(protocol::MOB_ACTION_TYPE_ATTACK)) {
    mob->ChangeState(protocol::MOB_ACTION_TYPE_STAND);
  }
}
