#include "pch.h"
#include "mob_stat.h"

void MobStat::UpdateStats() {
  _physical_attack = _base_physical_attack;
  _physical_defense = _base_physical_defense;
  _magical_attack = _base_magical_attack;
  _magical_defense = _base_magical_defense;

  _is_dirty = false;
}
