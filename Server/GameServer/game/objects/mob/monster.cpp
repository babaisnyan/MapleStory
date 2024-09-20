#include "pch.h"
#include "monster.h"

#include "data/spawn_point.h"
#include "data/templates/mob_template.h"

#include "game/objects/player/player.h"

#include "state/attack_state.h"
#include "state/hit_state.h"
#include "state/move_state.h"
#include "state/regen_state.h"
#include "state/stand_state.h"

#include "utils/randomizer.h"

#include <magic_enum.hpp>

static constexpr std::array kMonsterActions = {
  protocol::MobActionType::MOB_ACTION_TYPE_STAND,
  protocol::MobActionType::MOB_ACTION_TYPE_MOVE,
  protocol::MobActionType::MOB_ACTION_TYPE_ATTACK,
  protocol::MobActionType::MOB_ACTION_TYPE_HIT,
  protocol::MobActionType::MOB_ACTION_TYPE_REGEN,
};

Monster::Monster(const std::shared_ptr<SpawnPoint>& spawn_point, const std::shared_ptr<MapInstance>& map) : GameObject(GetNextObjectId()), _spawn_point(spawn_point), _map(map) {
  std::call_once(_init_flag, InitMonsterStates);
}

void Monster::Init(const std::shared_ptr<MobTemplate>& mob_template) {
  _id = mob_template->GetId();
  _mob_template = mob_template;
  _speed = (1 + static_cast<float>(mob_template->GetSpeed()) / 100) * 70;

  UpdatePosition(_spawn_point->GetX(), _spawn_point->GetY(), utils::random::IsSuccess(50));

  for (auto monster_action : kMonsterActions) {
    if (_mob_template->HasAction(monster_action)) {
      _states[monster_action] = _monster_states[monster_action];
    }
  }
}

void Monster::OnEnter() {
  if (_mob_template->HasAction(protocol::MOB_ACTION_TYPE_REGEN)) {
    ChangeState(protocol::MobActionType::MOB_ACTION_TYPE_REGEN);
  } else {
    ChangeState(protocol::MobActionType::MOB_ACTION_TYPE_STAND);
  }
}

void Monster::Update(const float delta_time) {
  _states[_current_state]->Update(GetSelf(), delta_time);
}

std::shared_ptr<Monster> Monster::GetSelf() {
  return std::static_pointer_cast<Monster>(shared_from_this());
}

uint32_t Monster::GetId() const {
  return _id;
}

std::shared_ptr<MobTemplate> Monster::GetTemplate() const {
  return _mob_template;
}

std::shared_ptr<SpawnPoint> Monster::GetSpawnPoint() const {
  return _spawn_point;
}

std::weak_ptr<MapInstance> Monster::GetMap() const {
  return _map;
}

std::weak_ptr<Player> Monster::GetTarget() const {
  return _target;
}

float Monster::GetSpeed() const {
  return _speed;
}

float Monster::GetAnimationTime() const {
  return _animation_time;
}

void Monster::AddAnimationTime(const float delta_time) {
  _animation_time += delta_time;
}

void Monster::ResetAnimationTime() {
  _animation_time = 0.0f;
}

void Monster::ResetTarget() {
  _target.reset();
}

void Monster::ChangeTarget(const std::shared_ptr<Player>& player) {
  _target = player;
  ResetTargetPosition();
}

bool Monster::IsTargetInDistance() const {
  const auto target = _target.lock();

  if (!target) {
    return false;
  }

  return _position.CheckTargetGridRange(target->GetPosition(), 3);
}

bool Monster::HasTarget() const {
  return !_target.expired();
}

void Monster::ResetTargetPosition() {
  _has_target_position = false;
}

void Monster::SetTargetPosition(const float x, const float y) {
  if (!_target_position) {
    _target_position = std::make_shared<MsCoordinate>();
  }

  _target_position->x = x;
  _target_position->y = y;
  _has_target_position = true;
}

bool Monster::HasTargetPosition() const {
  return _has_target_position;
}

std::shared_ptr<MsCoordinate> Monster::GetTargetPosition() const {
  return _target_position;
}

bool Monster::IsTargetInAttackRange() const {
  if (!HasTarget()) {
    return false;
  }

  if (!_mob_template->HasAction(protocol::MOB_ACTION_TYPE_ATTACK)) {
    return false;
  }

  if (_mob_template->GetAttackWidth() == 0 || _mob_template->GetAttackHeight() == 0) {
    return false;
  }

  const auto target = _target.lock();
  const auto range_x = _mob_template->GetAttackWidth();
  const auto range_y = _mob_template->GetAttackHeight();

  return _position.CheckInRangeDirection(target->GetPosition(), range_x, range_y, !_flip);
}

bool Monster::IsAttackReady() const {
  return GetTickCount64() >= _next_attack_time;
}

bool Monster::IsCollisionEnabled() const {
  return _is_collision_enabled;
}

void Monster::SetCollisionEnabled(const bool enabled) { // TODO: 패킷 보내기?
  _is_collision_enabled = enabled;
}

uint64_t Monster::GetRegenEndTime() const {
  return _regen_end_time;
}

void Monster::SetRegenEndTime(const uint64_t regen_end_time) {
  _regen_end_time = regen_end_time;
}

protocol::MobActionType Monster::GetCurrentState() const {
  return _current_state;
}

int64_t Monster::GetNextObjectId() {
  static std::atomic next_object_id = static_cast<int64_t>(kObjectRange) * static_cast<int32_t>(ObjectType::kMob);
  return next_object_id.fetch_add(1);
}

void Monster::ChangeState(const protocol::MobActionType state) {
  ASSERT_CRASH(_mob_template->HasAction(state));

  // std::cout << std::format("[Transition] ObjectId: {}, MobId: {}, {} -> {}\n", _object_id, _id, magic_enum::enum_name(_current_state), magic_enum::enum_name(state)).c_str();

  _current_state = state;
  _states[_current_state]->Enter(GetSelf());
}

void Monster::InitMonsterStates() {
  _monster_states.emplace(protocol::MobActionType::MOB_ACTION_TYPE_STAND, std::make_shared<StandState>());
  _monster_states.emplace(protocol::MobActionType::MOB_ACTION_TYPE_MOVE, std::make_shared<MoveState>());
  _monster_states.emplace(protocol::MobActionType::MOB_ACTION_TYPE_HIT, std::make_shared<HitState>());
  _monster_states.emplace(protocol::MobActionType::MOB_ACTION_TYPE_ATTACK, std::make_shared<AttackState>());
  _monster_states.emplace(protocol::MobActionType::MOB_ACTION_TYPE_REGEN, std::make_shared<RegenState>());
}
