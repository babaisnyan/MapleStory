#include "pch.h"
#include "migration_storage.h"

#include "network/center_session.h"

using Accessor = tbb::concurrent_hash_map<int32_t, std::shared_ptr<MigrationData>>::accessor;
using ConstAccessor = tbb::concurrent_hash_map<int32_t, std::shared_ptr<MigrationData>>::const_accessor;

void MigrationStorage::Add(const int32_t character_id, const std::shared_ptr<MigrationData>& data) {
  Accessor accessor;

  if (_migration_sessions.insert(accessor, character_id)) {
    accessor->second = data;
  }
}

void MigrationStorage::Remove(const int32_t character_id) {
  Accessor accessor;

  if (_migration_sessions.find(accessor, character_id)) {
    _migration_sessions.erase(accessor);
  }
}

void MigrationStorage::Remove(const std::shared_ptr<CenterSession>& session) {
  Accessor accessor;
  std::vector<int32_t> to_remove;

  for (auto& [character_id, data] : _migration_sessions) {
    const auto from_session = data->from_session.lock();
    const auto to_session = data->to_session.lock();

    if (!from_session || !to_session) {
      to_remove.push_back(character_id);
      continue;
    }

    if (from_session->GetSessionId() == session->GetSessionId() || to_session->GetSessionId() == session->GetSessionId()) {
      to_remove.push_back(character_id);
    }
  }

  for (const auto& character_id : to_remove) {
    _migration_sessions.erase(character_id);
  }
}

std::optional<std::shared_ptr<MigrationData>> MigrationStorage::Find(const int32_t character_id) const {
  ConstAccessor accessor;

  if (_migration_sessions.find(accessor, character_id)) {
    return std::make_optional(accessor->second);
  }

  return std::nullopt;
}
