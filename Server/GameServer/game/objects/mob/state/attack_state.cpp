#include "pch.h"
#include "attack_state.h"

#include "game/objects/mob/monster.h"

void AttackState::Enter(const std::shared_ptr<Monster>& mob) {
  mob->ResetAnimationTime();
}

void AttackState::Update(const std::shared_ptr<Monster>& mob, const float delta) {
  mob->AddAnimationTime(delta);

  if (mob->GetTemplate()->CanBodyAttack()) {
    ProcessCollision(mob);
  }

  // TODO: 애니메이션 시간 체크 후 공격 판정

    // TODO: 공격 쿨타임 설정
  mob->ChangeState(protocol::MOB_ACTION_TYPE_STAND);
}