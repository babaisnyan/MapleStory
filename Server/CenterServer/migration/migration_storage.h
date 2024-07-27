#pragma once
#include "data/migration_data.h"

namespace center {
  class MigrationStorage {
  public:
    static MigrationStorage& GetInstance() {
      static MigrationStorage instance;
      return instance;
    }

    void Add(int32_t character_id, const std::shared_ptr<MigrationData>& data);
    void Remove(int32_t character_id);
    void Remove(const std::shared_ptr<CenterSession>& session);
    std::optional<std::shared_ptr<MigrationData>> Find(int32_t character_id) const;

  private:
    tbb::concurrent_hash_map<int32_t, std::shared_ptr<MigrationData>> _migration_sessions;

    DECLARE_ACCESSOR_TYPE(_migration_sessions);
    DECLARE_CONST_ACCESSOR_TYPE(_migration_sessions);
  };
}
