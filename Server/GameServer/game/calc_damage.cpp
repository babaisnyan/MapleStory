#include "pch.h"
#include "calc_damage.h"

#include "objects/mob/mob_stat.h"
#include "objects/player/player_stat.h"

#include "utils/randomizer.h"

int32_t CalcDamage::CalcMobPhysicalDamage(const std::shared_ptr<MobStat>& mob_stat, const std::shared_ptr<PlayerStat>& player_stat) {
  const auto mob_pad = min(1999, max(0, mob_stat->GetPhysicalAttack() * 1.2));
  const auto low_damage = mob_pad * 0.8;
  const auto high_damage = mob_pad * 0.85;
  const auto calc = utils::random::RandDouble(low_damage, high_damage) * (mob_pad * 0.01);
  const auto player_level = player_stat->GetLevel();
  const auto mob_level = mob_stat->GetLevel();
  const auto player_pdd = player_stat->GetPhysicalDefense() * 2.5;
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

int32_t CalcDamage::CalcMobMagicalDamage(const std::shared_ptr<MobStat>& mob_stat, const std::shared_ptr<PlayerStat>& player_stat) {
  const auto mob_mad = max(0, mob_stat->GetMagicalAttack() * 2);
  const auto low_damage = mob_mad * 0.75;
  const auto high_damage = mob_mad * 0.8;
  const auto calc = utils::random::RandDouble(low_damage, high_damage) * (mob_mad * 0.01);
  const auto player_mdd = max(0, player_stat->GetMagicalDefense() * 2.5);
  const auto reduce = player_stat->GetStr() * 0.14285714285714 + player_stat->GetLuk() * 0.2 + player_stat->GetDex() * 0.1666666666667 + player_mdd;
  const auto damage = static_cast<int32_t>(max(calc - reduce, 0));

  return damage;
}

int32_t CalcDamage::CalcPlayerPhysicalDamage(const std::shared_ptr<PlayerStat>& player_stat, const std::shared_ptr<MobStat>& mob_stat) {
  const auto mob_pdd = max(mob_stat->GetPhysicalDefense(), mob_stat->GetLevel());
  const auto player_pad = (player_stat->GetPhysicalAttack() + player_stat->GetStatBase()) * 2;
  const auto low_damage = max(0, (player_pad - mob_pdd)) * 0.7;
  const auto high_damage = max(0, (player_pad - mob_pdd)) * 1.3;
  const auto calc = utils::random::RandDouble(low_damage, high_damage);

  return static_cast<int32_t>(max(calc - mob_pdd, 0));
}
