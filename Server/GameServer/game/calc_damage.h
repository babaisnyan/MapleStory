#pragma once

namespace game {
  class PlayerStat;
  class MobStat;

  class CalcDamage {
  public:
    static CalcDamage& GetInstance() {
      static CalcDamage instance;
      return instance;
    }

    int32_t CalcMobPhysicalDamage(const std::shared_ptr<MobStat>& mob_stat, const std::shared_ptr<PlayerStat>& player_stat);
    int32_t CalcMobMagicalDamage(const std::shared_ptr<MobStat>& mob_stat, const std::shared_ptr<PlayerStat>& player_stat);
    int32_t CalcPlayerPhysicalDamage(const std::shared_ptr<PlayerStat>& player_stat, const std::shared_ptr<MobStat>& mob_stat);
  };
}
