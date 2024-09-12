#pragma once

#include "mob_state.h"

namespace game {
  class RegenState final : public MobState {
  public:
    RegenState() = default;
    ~RegenState() override = default;

  public:
    RegenState(const RegenState&) = delete;
    RegenState& operator=(RegenState&) = delete;
    RegenState(RegenState&&) = delete;
    RegenState& operator=(RegenState&&) = delete;

    void Enter(const std::shared_ptr<Monster>& mob) override;
    void Update(const std::shared_ptr<Monster>& mob, float delta) override;

  private:
  };
}
