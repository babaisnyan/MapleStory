#pragma once

namespace game {
  class Player;

  class PlayerManager {
  public:
    static PlayerManager& GetInstance() {
      static PlayerManager instance;
      return instance;
    }

  public:
    void AddPlayer(const std::shared_ptr<Player>& player);
    void RemovePlayer(int32_t player_id);
    std::optional<std::shared_ptr<Player>> Find(int32_t player_id) const;

  private:
    tbb::concurrent_hash_map<int32_t, std::shared_ptr<Player>> _players;

    DECLARE_ACCESSOR_TYPE(_players);
    DECLARE_CONST_ACCESSOR_TYPE(_players);
  };
}
