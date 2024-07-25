#include "pch.h"
#include "migration_storage.h"

#include "network/login/login_session.h"

using Accessor = tbb::concurrent_hash_map<int32_t, std::weak_ptr<LoginSession>>::accessor;
using ConstAccessor = tbb::concurrent_hash_map<int32_t, std::weak_ptr<LoginSession>>::const_accessor;

void MigrationStorage::Add(const int32_t character_id, const std::weak_ptr<LoginSession>& session) {
  Accessor accessor;

  if (_migration_sessions.insert(accessor, character_id)) {
    accessor->second = session;
  }
}

void MigrationStorage::Remove(const int32_t character_id) {
  Accessor accessor;

  if (_migration_sessions.find(accessor, character_id)) {
    _migration_sessions.erase(accessor);
  }
}

void MigrationStorage::Remove(const std::shared_ptr<LoginSession>& session) {
  Accessor accessor;

  for (auto& [character_id, weak_session] : _migration_sessions) {
    const auto shared_session = weak_session.lock();

    if (shared_session->GetSessionId() == session->GetSessionId()) {
      _migration_sessions.erase(character_id);
      break;
    }
  }
}

std::optional<std::weak_ptr<LoginSession>> MigrationStorage::Find(const int32_t character_id) const {
  ConstAccessor accessor;

  if (_migration_sessions.find(accessor, character_id)) {
    return std::make_optional(accessor->second);
  }

  return std::nullopt;
}
