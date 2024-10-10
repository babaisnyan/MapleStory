#include "pch.h"
#include "calc_damage.h"

#include "objects/mob/mob_stat.h"
#include "objects/player/player_stat.h"

#include "utils/randomizer.h"

int32_t CalcDamage::CalcMobPhysicalDamage(const std::shared_ptr<MobStat>& mob_stat, const std::shared_ptr<PlayerStat>& player_stat) {
  const auto mob_pad = min(1999, max(0, mob_stat->GetPhysicalAttack() * 2));
  const auto low_damage = mob_pad * 0.8;
  const auto high_damage = mob_pad * 0.85;
  const auto calc = utils::random::RandDouble(low_damage, high_damage) * (mob_pad * 0.01);
  const auto player_level = player_stat->GetLevel();
  const auto mob_level = mob_stat->GetLevel();
  const auto player_pdd = player_stat->GetPhysicalDefense();
  const auto player_stat_base = player_stat->GetStatBase();
  double calc1 = player_stat_base * 0.00125;
  double calc2;

  if (player_pdd > mob_pad) {
    calc1 = player_level * 0.001818181818181818 + player_stat_base * 0.00125 + 0.28;

    if (player_level >= mob_level) {
      calc2 = calc1 * (mob_pad - player_pdd) * 13.0 / (player_level - mob_level + 13.0);
    } else {
      calc2 = calc1 * (mob_pad - player_pdd) * 1.3;
    }
  } else {
    calc2 = (player_stat_base * 0.0011111111111111 + player_level * 0.0007692307692307692 + 0.28) * (mob_pad - player_pdd) * 0.7;
  }

  const auto damage = static_cast<int32_t>(max(calc - (calc2 + (calc1 + 0.28) * mob_pad) - (calc - (calc2 + (calc1 + 0.28) * mob_pad)) * 0.01, 0.0));

  return damage;
}

int32_t CalcDamage::CalcMobMagicalDamage(std::shared_ptr<MobStat> mob_stat, std::shared_ptr<PlayerStat> player_stat) {
  return 0;
}

int32_t CalcDamage::CalcPlayerPhysicalDamage(std::shared_ptr<PlayerStat> player_stat, std::shared_ptr<MobStat> mob_stat) {
  return 0;
}

int32_t CalcDamage::CalcPlayerMagicalDamage(std::shared_ptr<PlayerStat> player_stat, std::shared_ptr<MobStat> mob_stat) {
  return 0;
}
