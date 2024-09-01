#pragma once

namespace game {
  class Player;
  class Monster;

  class MobState { //TODO: BT 구현
  public:
    virtual ~MobState() = default;

  public:
    virtual void Enter(std::shared_ptr<Monster> mob) = 0;
    virtual void Update(std::shared_ptr<Monster> mob, float delta) = 0;
    virtual void SecondUpdate(std::shared_ptr<Monster> mob, float delta) = 0;

  protected:
    std::shared_ptr<Player> FindNearestPlayer(std::shared_ptr<Monster> mob);
  };
}
