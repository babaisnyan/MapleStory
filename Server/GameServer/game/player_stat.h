#pragma once
#include "basic_stat.h"

namespace game {
  class PlayerStat final : public BasicStat {
  public:
    int32_t GetStr() const;
    void SetStr(int32_t str);

    int32_t GetDex() const;
    void SetDex(int32_t dex);

    int32_t GetInt() const;
    void SetInt(int32_t int_);

    int32_t GetLuk() const;
    void SetLuk(int32_t luk);

    int32_t GetExp() const;
    void SetExp(int32_t exp);

    int32_t GetAp() const;
    void SetAp(int32_t ap);

    int32_t GetSp() const;
    void SetSp(int32_t sp);

  private:
    int32_t _str = 0;
    int32_t _dex = 0;
    int32_t _int = 0;
    int32_t _luk = 0;
    int32_t _exp = 0;
    int32_t _ap = 0;
    int32_t _sp = 0;
  };
}
