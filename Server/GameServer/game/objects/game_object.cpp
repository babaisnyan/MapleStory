#include "pch.h"
#include "game_object.h"

GameObject::GameObject(const int64_t object_id) : _object_id(object_id) {}

int64_t GameObject::GetObjectId() const {
  return _object_id;
}
