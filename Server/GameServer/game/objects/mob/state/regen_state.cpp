#include "pch.h"
#include "regen_state.h"

#include "game/objects/mob/monster.h"

void RegenState::Enter(const std::shared_ptr<Monster>& mob) {
  mob->ResetAnimationTime();
  mob->SetCollisionEnabled(false);
  mob->SetRegenEndTime(GetTickCount64() + mob->GetTemplate()->GetActionLength(protocol::MOB_ACTION_TYPE_REGEN));
}

void RegenState::Update(const std::shared_ptr<Monster>& mob, const float delta) {
  mob->AddAnimationTime(delta);

  if (GetTickCount64() >= mob->GetRegenEndTime()) {
    mob->SetCollisionEnabled(true);
    mob->ChangeState(protocol::MobActionType::MOB_ACTION_TYPE_STAND);
  }
}