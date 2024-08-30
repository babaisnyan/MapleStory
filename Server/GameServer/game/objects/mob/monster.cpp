#include "pch.h"
#include "monster.h"

#include "data/spawn_point.h"
#include "data/templates/mob_template.h"

Monster::Monster(const std::shared_ptr<SpawnPoint>& spawn_point, const std::shared_ptr<MapInstance>& map) : GameObject(GetNextObjectId()), _spawn_point(spawn_point), _map(map) {}

void Monster::Init(const std::shared_ptr<MobTemplate>& mob_template) {
  _id = mob_template->GetId();
  _mob_template = mob_template;

  UpdatePosition(_spawn_point->GetX(), _spawn_point->GetY());
}


void Monster::OnEnter() {}

void Monster::Update(float delta_time) {}

void Monster::SecondUpdate(float delta_time) {}

void Monster::UpdatePosition(const float x, const float y) {
  _position.x = x;
  _position.y = y;
  _position.grid_x = static_cast<int32_t>(x) / _position.grid_size;
  _position.grid_y = static_cast<int32_t>(y) / _position.grid_size;
}

uint32_t Monster::GetId() const {
  return _id;
}

const MsCoordinate& Monster::GetPosition() const {
  return _position;
}

float Monster::GetX() const {
  return _position.x;
}

float Monster::GetY() const {
  return _position.y;
}

int64_t Monster::GetNextObjectId() {
  static std::atomic<int64_t> next_object_id = static_cast<int32_t>(kObjectRange) * static_cast<int32_t>(ObjectType::kMob);
  return next_object_id.fetch_add(1);
}
