#pragma once
#include "mob_state.h"

namespace game {
  class WalkState final : public MobState {
  public:
    ~WalkState() override = default;

    void Enter(std::shared_ptr<Monster> mob) override;
    void Update(std::shared_ptr<Monster> mob, float delta) override;
    void SecondUpdate(std::shared_ptr<Monster> mob, float delta) override;

  private:
  };
}
