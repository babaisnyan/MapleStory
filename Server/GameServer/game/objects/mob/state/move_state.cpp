#include "pch.h"
#include "move_state.h"

#include "game/map/map_instance.h"
#include "game/objects/mob/monster.h"

#include "utils/randomizer.h"

void MoveState::Enter(const std::shared_ptr<Monster>& mob) {
  mob->ResetAnimationTime();

  protocol::GameServerMobMove move;
  move.set_object_id(mob->GetObjectId());
  move.set_x(mob->GetX());
  move.set_y(mob->GetY());
  move.set_flip(mob->IsFlipped());
  move.set_state(mob->GetCurrentState());

  mob->GetMap().lock()->BroadCast(move, nullptr);
}

void MoveState::Update(const std::shared_ptr<Monster>& mob, const float delta) {
  mob->AddAnimationTime(delta);
  // TODO: 캐릭터 타겟 탐색

  const auto dir = mob->IsFlipped() ? -1.0f : 1.0f;
  const auto test = static_cast<int32_t>(dir * (mob->GetSpeed() * delta));
  const auto x = mob->GetX() + static_cast<int32_t>(dir * (mob->GetSpeed() * delta));
  const auto y = mob->GetY();
  const auto min_x = mob->GetSpawnPoint()->GetMinX();
  const auto max_x = mob->GetSpawnPoint()->GetMaxX();

  if (static_cast<int32_t>(x) > min_x && static_cast<int32_t>(x) < max_x) {
    mob->UpdatePosition(x, y, mob->IsFlipped());

    protocol::GameServerMobMove move;
    move.set_object_id(mob->GetObjectId());
    move.set_x(x);
    move.set_y(y);
    move.set_flip(mob->IsFlipped());
    move.set_state(mob->GetCurrentState());
    mob->GetMap().lock()->BroadCast(move, nullptr);

    std::cout << std::format("Monster {} move to x: {}, y: {}\n", mob->GetId(), x, y);
  } else {
    mob->SetFlip(!mob->IsFlipped());
  }
}

void MoveState::PostUpdate(const std::shared_ptr<Monster>& mob) {
  if (mob->HasTarget()) {
    if (mob->IsAttackReady() && mob->IsTargetInDistance()) {
      mob->ChangeState(protocol::MOB_ACTION_TYPE_ATTACK);
    }
  } else {
    protocol::GameServerMobMove move;

    switch (utils::random::Rand(150)) {
      case 50:
        mob->SetFlip(false);

        move.set_object_id(mob->GetObjectId());
        move.set_x(mob->GetX());
        move.set_y(mob->GetY());
        move.set_flip(mob->IsFlipped());
        move.set_state(mob->GetCurrentState());

        mob->GetMap().lock()->BroadCast(move, nullptr);
        break;
      case 100:
        mob->SetFlip(true);

        move.set_object_id(mob->GetObjectId());
        move.set_x(mob->GetX());
        move.set_y(mob->GetY());
        move.set_flip(mob->IsFlipped());
        move.set_state(mob->GetCurrentState());

        mob->GetMap().lock()->BroadCast(move, nullptr);
        break;
      case 149:
        mob->ChangeState(protocol::MOB_ACTION_TYPE_STAND);
        break;
    }
  }
}
