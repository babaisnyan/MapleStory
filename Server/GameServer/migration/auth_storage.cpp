#include "pch.h"
#include "auth_storage.h"


void AuthStorage::Add(const int32_t player_id, const int32_t auth_key) {
  Accessor accessor;

  if (_auth_keys.insert(accessor, player_id)) {
    accessor->second = auth_key;
  }
}

void AuthStorage::Remove(const int32_t player_id) {
  Accessor accessor;

  if (_auth_keys.find(accessor, player_id)) {
    _auth_keys.erase(accessor);
  }
}

std::optional<int32_t> AuthStorage::Find(const int32_t player_id) const {
  ConstAccessor accessor;

  if (_auth_keys.find(accessor, player_id)) {
    return std::make_optional(accessor->second);
  }

  return std::nullopt;
}

bool AuthStorage::IsExist(const int32_t player_id) const {
  ConstAccessor accessor;

  return _auth_keys.find(accessor, player_id);
}
