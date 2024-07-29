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

  private:
    int32_t _str = 0;
    int32_t _dex = 0;
    int32_t _int = 0;
    int32_t _luk = 0;
  };
}
