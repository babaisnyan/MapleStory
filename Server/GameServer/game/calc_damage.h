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
    int32_t CalcMobMagicalDamage(std::shared_ptr<MobStat> mob_stat, std::shared_ptr<PlayerStat> player_stat);
    int32_t CalcPlayerPhysicalDamage(std::shared_ptr<PlayerStat> player_stat, std::shared_ptr<MobStat> mob_stat);
    int32_t CalcPlayerMagicalDamage(std::shared_ptr<PlayerStat> player_stat, std::shared_ptr<MobStat> mob_stat);
  };
}
