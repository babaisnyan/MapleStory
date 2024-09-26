#include "pch.h"
#include "game_object.h"

GameObject::GameObject(const int64_t object_id) : _object_id(object_id) {}

int64_t GameObject::GetObjectId() const {
  return _object_id;
}

GameObject::ObjectType GameObject::GetObjectType() const {
  return static_cast<ObjectType>(_object_id / kObjectRange);
}

const MsCoordinate& GameObject::GetPosition() const {
  return _position;
}

float GameObject::GetX() const {
  return _position.x;
}

float GameObject::GetY() const {
  return _position.y;
}

bool GameObject::IsFlipped() const {
  return _flip;
}

void GameObject::SetFlip(const bool flip) {
  _flip = flip;
}

void GameObject::SetBounds(const std::tuple<int32_t, int32_t, int32_t, int32_t>& bounds) {
  _position.bounds = bounds;
}

void GameObject::UpdatePosition(const float x, const float y, const bool flip) {
  _position.x = x;
  _position.y = y;

  if (x < 0) {
    _position.grid_x = (x - std::get<0>(_position.bounds)) / MsCoordinate::kGridSize;
  } else {
    _position.grid_x = (std::abs(std::get<0>(_position.bounds)) + x) / MsCoordinate::kGridSize;
  }

  _position.grid_y = std::abs(static_cast<int16_t>(_position.y) / MsCoordinate::kGridSize);
  _flip = flip;
}
