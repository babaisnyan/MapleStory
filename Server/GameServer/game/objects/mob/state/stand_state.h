#pragma once

#include "mob_state.h"

namespace game {
  class StandState final : public MobState {
  public:
    StandState() = default;
    ~StandState() override = default;

    StandState(const StandState&) = delete;
    StandState& operator=(StandState&) = delete;

    void Enter(const std::shared_ptr<Monster>& mob) override;
    void Update(const std::shared_ptr<Monster>& mob, float delta) override;

  private:
  };
}
