#pragma once

namespace game {
  class BasicStat : std::enable_shared_from_this<BasicStat> {
  public:
    virtual ~BasicStat() = default;

    virtual void UpdateStats() = 0;

    bool IsDirty() const;

    int16_t GetLevel() const;
    void SetLevel(int16_t level);

    uint64_t GetExp() const;
    void SetExp(uint64_t exp);

    int32_t GetHp() const;
    void SetHp(int32_t hp);

    int32_t GetMaxHp() const;
    void SetMaxHp(int32_t max_hp);

    double GetPhysicalAttack() const;
    double GetPhysicalDefense() const;
    double GetMagicalAttack() const;
    double GetMagicalDefense() const;

    void SetBasePhysicalAttack(double base_physical_attack);
    void SetBasePhysicalDefense(double base_physical_defense);
    void SetBaseMagicalAttack(double base_magical_attack);
    void SetBaseMagicalDefense(double base_magical_defense);

  protected:
    bool _is_dirty = true;

    /*Static Stats*/
    int16_t _level = 0;
    uint64_t _exp = 0;
    int32_t _hp = 0;
    int32_t _max_hp = 0;

    /*Dynamic Stats*/
    double _base_physical_attack = 0;
    double _base_physical_defense = 0;
    double _base_magical_attack = 0;
    double _base_magical_defense = 0;
    double _physical_attack = 0;
    double _physical_defense = 0;
    double _magical_attack = 0;
    double _magical_defense = 0;
  };
}
