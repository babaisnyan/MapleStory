#include "pch.h"
#include "move_state.h"

#include "random.h"

#include "game/objects/mob/monster.h"

void MoveState::Enter(const std::shared_ptr<Monster>& mob) {
  mob->ResetAnimationTime();
}

void MoveState::Update(const std::shared_ptr<Monster>& mob, const float delta) {
  mob->AddAnimationTime(delta);
  // TODO: 벽 충돌 체크, 이동, 캐릭터 타겟 탐색
}

void MoveState::PostUpdate(const std::shared_ptr<Monster>& mob) {
  if (mob->HasTarget()) {
    if (mob->IsAttackReady() && mob->IsTargetInDistance()) {
      mob->ChangeState(protocol::MOB_ACTION_TYPE_ATTACK);
    }
  } else {
    if (mob->GetAnimationTime() > 5.0f || random::IsSuccess(15)) {
      mob->ChangeState(protocol::MOB_ACTION_TYPE_STAND);
    }
  }
}
