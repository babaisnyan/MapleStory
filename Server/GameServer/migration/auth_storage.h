#pragma once

namespace game {
  class AuthStorage {
  public:
    static AuthStorage& GetInstance() {
      static AuthStorage instance;
      return instance;
    }

    void Add(int32_t player_id, int32_t auth_key);
    void Remove(int32_t player_id);
    std::optional<int32_t> Find(int32_t player_id) const;
    bool IsExist(int32_t player_id) const;

  private:
    tbb::concurrent_hash_map<int32_t, int32_t> _auth_keys;

    DECLARE_ACCESSOR_TYPE(_auth_keys);
    DECLARE_CONST_ACCESSOR_TYPE(_auth_keys);
  };
}
