#pragma once

#include "mob_state.h"

namespace game {
  class MoveState final : public MobState {
  public:
    MoveState() = default;
    ~MoveState() override = default;

  public:
    MoveState(const MoveState&) = delete;
    MoveState& operator=(MoveState&) = delete;
    MoveState(MoveState&&) = delete;
    MoveState& operator=(MoveState&&) = delete;

    void Enter(const std::shared_ptr<Monster>& mob) override;
    void Update(const std::shared_ptr<Monster>& mob, float delta) override;
  };
}
