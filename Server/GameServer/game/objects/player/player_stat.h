﻿#pragma once

#include "game/basic_stat.h"

namespace game {
  class Inventory;
}

namespace game {
  class PlayerStat final : public BasicStat {
  public:
    void UpdateStats() override;
    void ApplyEquip(const std::shared_ptr<Inventory>& inventory);

    int32_t GetStr() const;
    void SetStr(int32_t str);

    int32_t GetDex() const;
    void SetDex(int32_t dex);

    int32_t GetInt() const;
    void SetInt(int32_t int_);

    int32_t GetLuk() const;
    void SetLuk(int32_t luk);

    int32_t GetAp() const;
    void SetAp(int32_t ap);

    int32_t GetSp() const;
    void SetSp(int32_t sp);

    int32_t GetBuffedMaxHp() const;

    int32_t GetMp() const;
    void SetMp(int32_t mp);

    int32_t GetMaxMp() const;
    void SetMaxMp(int32_t max_mp);

    uint64_t GetLastCollisionTime() const;
    void SetLastCollisionTime(uint64_t last_collision_time);

    double GetStatBase() const;

    uint64_t GetMaxExp() const;

  private:
    /*Static Stats*/
    int32_t _mp = 0;
    int32_t _max_mp = 0;
    int32_t _str = 0;
    int32_t _dex = 0;
    int32_t _int = 0;
    int32_t _luk = 0;
    int32_t _ap = 0;
    int32_t _sp = 0;
    uint64_t _last_collision_time = 0;

    /*Dynamic Stats*/
    int32_t _extra_str = 0;
    int32_t _extra_dex = 0;
    int32_t _extra_int = 0;
    int32_t _extra_luk = 0;
    int32_t _extra_pad = 0;
    int32_t _extra_pdd = 0;
    int32_t _extra_mad = 0;
    int32_t _extra_mdd = 0;
    int32_t _extra_hp = 0;
    double _stat_base = 0.0;
  };
}
