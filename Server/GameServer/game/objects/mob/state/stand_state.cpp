#include "pch.h"
#include "stand_state.h"

#include "game/objects/mob/monster.h"

void StandState::Enter(const std::shared_ptr<Monster>& mob) {
}

void StandState::Update(const std::shared_ptr<Monster>& mob, const float delta) {

  // TODO: 현재 타겟이 있다면 거리 계산 후 일정 거리 이상이면 타겟 재설정
}

void StandState::PostUpdate(const std::shared_ptr<Monster>& mob) {
  // TODO: 타겟이 있을때 타겟과의 거리 계산 후 공격 가능하면 공격 상태로 변경
}
