#include "pch.h"
#include "move_state.h"

#include "game/objects/mob/monster.h"

#include "utils/game_math.h"
#include "utils/randomizer.h"

void MoveState::Enter(const std::shared_ptr<Monster>& mob) {
  mob->ResetAnimationTime();
}

void MoveState::Update(const std::shared_ptr<Monster>& mob, const float delta) {
  mob->AddAnimationTime(delta);
  // TODO: 벽 충돌 체크, 이동, 캐릭터 타겟 탐색

  const auto dir = mob->IsFlipped() ? -1 : 1;
  auto x = mob->GetX() + math::Sin256(dir) * mob->GetSpeed();
  auto y = mob->GetY() - math::Cos256(dir) * mob->GetSpeed();
}

void MoveState::PostUpdate(const std::shared_ptr<Monster>& mob) {
  if (mob->HasTarget()) {
    if (mob->IsAttackReady() && mob->IsTargetInDistance()) {
      mob->ChangeState(protocol::MOB_ACTION_TYPE_ATTACK);
    }
  } else {
    switch (utils::random::Rand(150)) {
      case 50:
        mob->SetFlip(false);
        break;
      case 100:
        mob->SetFlip(true);
        break;
      case 149:
        mob->ChangeState(protocol::MOB_ACTION_TYPE_STAND);
        break;
    }
  }
}
