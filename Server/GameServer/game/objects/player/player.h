#pragma once

#include "game/objects/game_object.h"

namespace game {
  class Inventory;
  class Monster;
  class KeyMap;
  class PlayerStat;

  class Player final : public GameObject {
  public:
    explicit Player(const int32_t player_id);

    void OnEnter() override;
    void Update(float delta_time) override;
    void OnCollideMob(const std::shared_ptr<Monster>& mob, uint64_t time);
    void OnDamage(int32_t damage, uint64_t time);
    void Kill();
    void Revive();
    void Attack();

  public:
    int32_t GetId() const;
    int32_t GetAccountId() const;
    const String& GetName() const;
    int8_t GetType() const;
    int16_t GetJob() const;
    void SetJob(int16_t job);
    int32_t GetMeso() const;
    void SetMeso(int32_t meso);
    int32_t GetMap() const;
    void SetMap(int32_t map);
    bool CanAttack() const;
    void AddExp(int32_t exp);

    std::shared_ptr<PlayerStat> GetStat() const;
    std::shared_ptr<KeyMap> GetKeyMap() const;
    std::shared_ptr<Inventory> GetInventory() const;

  public:
    bool TryLoadFromDb();
    bool TrySaveToDb();

  protected:
    int64_t GetNextObjectId() override;

  private:
    int32_t _id = 0;
    int32_t _account_id = 0;
    String _name;
    int8_t _type = 0;
    int16_t _job = 0;
    int32_t _meso = 0;
    int32_t _map = 0;
    uint64_t _last_attack_time = 0;
    std::shared_ptr<PlayerStat> _player_stat = nullptr;
    std::shared_ptr<KeyMap> _key_map = nullptr;
    std::shared_ptr<Inventory> _inventory = nullptr;
  };
}
