#include "pch.h"
#include "attack_state.h"

#include "game/objects/mob/monster.h"

void AttackState::Enter(const std::shared_ptr<Monster>& mob) {}

void AttackState::Update(const std::shared_ptr<Monster>& mob, const float delta) {
  if (mob->GetTemplate()->CanBodyAttack()) {
    ProcessCollision(mob);
  }

  if (mob->IsAttackReady()) {
    mob->Attack();
  }

  if (mob->GetAnimationTime() >= mob->GetTemplate()->GetActionLength(protocol::MOB_ACTION_TYPE_ATTACK)) {
    mob->ChangeState(protocol::MOB_ACTION_TYPE_STAND);
  }
}
