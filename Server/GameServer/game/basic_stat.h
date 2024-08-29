#pragma once

namespace game {
  class BasicStat : std::enable_shared_from_this<BasicStat> {
  public:
    virtual ~BasicStat() = default;

    int16_t GetLevel() const;
    void SetLevel(int16_t level);

    int32_t GetHp() const;
    void SetHp(int32_t hp);

    int32_t GetMaxHp() const;
    void SetMaxHp(int32_t max_hp);

  private:
    int16_t _level = 0;
    int32_t _hp = 0;
    int32_t _max_hp = 0;
  };
}
