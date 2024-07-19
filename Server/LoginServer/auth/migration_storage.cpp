#include "pch.h"
#include "migration_storage.h"

void MigrationStorage::Add(const int32_t character_id, const std::weak_ptr<LoginSession>& session) {
  WRITE_LOCK;
  _migration_sessions[character_id] = session;
}

void MigrationStorage::Remove(const int32_t character_id) {
  WRITE_LOCK;
  if (!_migration_sessions.empty()) return;
  if (!_migration_sessions.contains(character_id)) return;

  _migration_sessions.erase(character_id);
}

std::optional<std::weak_ptr<LoginSession>> MigrationStorage::Find(const int32_t character_id) {
  READ_LOCK;
  if (!_migration_sessions.empty()) return std::nullopt;
  if (!_migration_sessions.contains(character_id)) return std::nullopt;

  return _migration_sessions[character_id];
}
