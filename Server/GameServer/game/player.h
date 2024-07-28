#pragma once
#include "network/game/game_session.h"

namespace game {
  class Player {
  public:
    Player(const int32_t id, const int32_t account_id, String name, const int8_t type, const int16_t level, const int32_t exp, const int32_t meso, const int32_t map, const int32_t hp, const int32_t mp, const int32_t max_hp, const int32_t max_mp, const int32_t str, const int32_t dex, const int32_t int_, const int32_t luk)
      : _id(id),
        _account_id(account_id),
        _name(std::move(name)),
        _type(type),
        _level(level),
        _exp(exp),
        _meso(meso),
        _map(map),
        _hp(hp),
        _mp(mp),
        _max_hp(max_hp),
        _max_mp(max_mp),
        _str(str),
        _dex(dex),
        _int(int_),
        _luk(luk) {}

    int32_t GetId() const;
    int32_t GetAccountId() const;
    const String& GetName() const;
    int8_t GetType() const;
    int16_t GetLevel() const;
    int16_t GetJob() const;
    int32_t GetExp() const;
    int32_t GetMeso() const;
    int32_t GetMap() const;
    int32_t GetHp() const;
    int32_t GetMp() const;
    int32_t GetMaxHp() const;
    int32_t GetMaxMp() const;
    int32_t GetStr() const;
    int32_t GetDex() const;
    int32_t GetInt() const;
    int32_t GetLuk() const;

    void SetSession(const GameSessionRef& shared);

  private:
    int32_t _id = 0;
    int32_t _account_id = 0;
    String _name;
    int8_t _type = 0;
    int16_t _level = 0;
    int16_t _job = 0;
    int32_t _exp;
    int32_t _meso;
    int32_t _map;
    int32_t _hp;
    int32_t _mp;
    int32_t _max_hp;
    int32_t _max_mp;
    int32_t _str;
    int32_t _dex;
    int32_t _int;
    int32_t _luk;

    GameSessionRef _session;
  };
}
