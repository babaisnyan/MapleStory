#pragma once
#include "game/basic_stat.h"

namespace game {
  class MobStat final : public BasicStat {
  public:
    void UpdateStats() override;

  private:
  };
}
