#pragma once

namespace login {
  class LoginSession;

  class MigrationStorage {
  public:
    static MigrationStorage& GetInstance() {
      static MigrationStorage instance;
      return instance;
    }

    void Add(int32_t character_id, const std::weak_ptr<LoginSession>& session);
    void Remove(int32_t character_id);
    void Remove(const std::shared_ptr<LoginSession>& session);
    std::optional<std::weak_ptr<LoginSession>> Find(int32_t character_id) const;

  private:
    tbb::concurrent_hash_map<int32_t, std::weak_ptr<LoginSession>> _migration_sessions;

    DECLARE_ACCESSOR_TYPE(_migration_sessions);
    DECLARE_CONST_ACCESSOR_TYPE(_migration_sessions);
  };
}
