#include "pch.h"
#include "game_object.h"

GameObject::GameObject() : _object_id(++next_object_id) {}

int64_t GameObject::GetObjectId() const {
   return _object_id;
}
