#pragma once

class LoginCharacter {
public:
  LoginCharacter(const int32_t id, const std::wstring& name, const int16_t level, const int8_t type, const int16_t job, const int16_t hp, const int16_t mp, const int16_t str, const int16_t dex, const int16_t luk, const int16_t int_)
    : _id(id), _name(name), _level(level), _type(type), _job(job), _hp(hp), _mp(mp), _str(str), _dex(dex), _luk(luk), _int(int_) {}

  int32_t             GetId() const { return _id; }
  const std::wstring& GetName() const { return _name; }
  int16_t             GetLevel() const { return _level; }
  int8_t              GetType() const { return _type; }
  int16_t             GetJob() const { return _job; }
  int16_t             GetHp() const { return _hp; }
  int16_t             GetMp() const { return _mp; }
  int16_t             GetStr() const { return _str; }
  int16_t             GetDex() const { return _dex; }
  int16_t             GetLuk() const { return _luk; }
  int16_t             GetInt() const { return _int; }

private:
  int32_t      _id;
  std::wstring _name;
  int16_t      _level;
  int8_t       _type;
  int16_t      _job;
  int16_t      _hp;
  int16_t      _mp;
  int16_t      _str;
  int16_t      _dex;
  int16_t      _luk;
  int16_t      _int;
};
