#include "pch.h"
#include "game_object.h"

GameObject::GameObject(const int64_t object_id) : _object_id(object_id) {}

int64_t GameObject::GetObjectId() const {
  return _object_id;
}

const MsCoordinate& GameObject::GetPosition() const {
  return _position;
}

int32_t GameObject::GetX() const {
  return _position.x;
}

int32_t GameObject::GetY() const {
  return _position.y;
}

bool GameObject::IsFlipped() const {
  return _flip;
}

void GameObject::SetFlip(const bool flip) {
  _flip = flip;
}

void GameObject::UpdatePosition(const int32_t x, const int32_t y, const bool flip) {
  if(abs(x - _position.x) > 100) {
    std::cout << "1\n";
  }

  _position.x = x;
  _position.y = y;
  _position.grid_x = static_cast<int16_t>(x) / MsCoordinate::kGridSize;
  _position.grid_y = static_cast<int16_t>(y) / MsCoordinate::kGridSize;
  _flip = flip;
}
