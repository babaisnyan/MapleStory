#pragma once

#include "mob_state.h"

namespace game {
  class HitState final : public MobState {
  public:
    HitState() = default;
    ~HitState() override = default;

  public:
    HitState(const HitState&) = delete;
    HitState& operator=(HitState&) = delete;
    HitState(HitState&&) = delete;
    HitState& operator=(HitState&&) = delete;

    void Enter(const std::shared_ptr<Monster>& mob) override;
    void Update(const std::shared_ptr<Monster>& mob, float delta) override;
  
  };
  
}
