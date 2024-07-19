#pragma once

class LoginSession;

class MigrationStorage {
public:
  static MigrationStorage& GetInstance() {
    static MigrationStorage instance;
    return instance;
  }

  void Add(int32_t character_id, const std::weak_ptr<LoginSession>& session);
  void Remove(int32_t character_id);
  std::optional<std::weak_ptr<LoginSession>> Find(int32_t character_id);

private:
  USE_LOCK;
  HashMap<uint32_t, std::weak_ptr<LoginSession>> _migration_sessions;
};
