#pragma once

namespace game {
  class Player;
  class Monster;

  class MobState {
  protected:
    MobState() = default;
     
  public:
    virtual ~MobState() = default;

    MobState(const MobState&) = delete;
    MobState& operator=(MobState&) = delete;

  public:
    virtual void Enter(const std::shared_ptr<Monster>& mob) = 0;
    virtual void Update(const std::shared_ptr<Monster>& mob, float delta) = 0;
    virtual void PostUpdate(const std::shared_ptr<Monster>& mob) = 0;

  protected:
    static std::shared_ptr<Player> FindNearestPlayer(const std::shared_ptr<Monster>& mob);
  };
}
