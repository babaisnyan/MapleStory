#include "pch.h"
#include "move_state.h"

#include "game/objects/mob/monster.h"

void MoveState::Enter(const std::shared_ptr<Monster>& mob) {

}

void MoveState::Update(const std::shared_ptr<Monster>& mob, const float delta) {


  // TODO: 벽 충돌 체크, 이동, 캐릭터 타겟 탐색
}

void MoveState::PostUpdate(const std::shared_ptr<Monster>& mob) {
  // TODO: 타겟이 있을때 타겟과의 거리 계산 후 공격 가능하면 공격 상태로 변경
}
