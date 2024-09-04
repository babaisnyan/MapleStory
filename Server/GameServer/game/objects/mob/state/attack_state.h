#pragma once

#include "mob_state.h"

namespace game {
  class AttackState final : public MobState {
  public:
    AttackState() = default;
    ~AttackState() override = default;

  public:
    AttackState(const AttackState&) = delete;
    AttackState& operator=(AttackState&) = delete;
    AttackState(AttackState&&) = delete;
    AttackState& operator=(AttackState&&) = delete;

    void Enter(const std::shared_ptr<Monster>& mob) override;
    void Update(const std::shared_ptr<Monster>& mob, float delta) override;
    void PostUpdate(const std::shared_ptr<Monster>& mob) override;
  };
}
