#pragma once
#include "types.h"
#include "database/db_bind.h"

#include <Windows.h>

namespace sp {
  class InsertGold : public DbBind<3, 0> {
  public:
    explicit InsertGold(DbConnection& conn) : DbBind(conn, L"{CALL dbo.spInsertGold(?,?,?)}") {}
    void     InGold(int32_t& v) { BindParam(0, v); }

    void InGold(int32_t&& v) {
      _gold = std::move(v);
      BindParam(0, _gold);
    }

    template <int32_t N>
    void InName(wchar_t (&v)[N]) { BindParam(1, v); }

    template <int32_t N>
    void InName(const wchar_t (&v)[N]) { BindParam(1, v); }

    void InName(wchar_t* v, const int32_t count) { BindParam(1, v, count); }
    void InName(const wchar_t* v, const int32_t count) { BindParam(1, v, count); }
    void InCreateDate(TIMESTAMP_STRUCT& v) { BindParam(2, v); }

    void InCreateDate(TIMESTAMP_STRUCT&& v) {
      _createDate = std::move(v);
      BindParam(2, _createDate);
    }

  private:
    int32_t          _gold = {};
    TIMESTAMP_STRUCT _createDate = {};
  };

  class GetGold : public DbBind<1, 4> {
  public:
    explicit GetGold(DbConnection& conn) : DbBind(conn, L"{CALL dbo.spGetGold(?)}") {}
    void     InGold(int32_t& v) { BindParam(0, v); }

    void InGold(int32_t&& v) {
      _gold = std::move(v);
      BindParam(0, _gold);
    }

    void OutId(int32_t& v) { BindCol(0, v); }
    void OutGold(int32_t& v) { BindCol(1, v); }

    template <int32_t N>
    void OutName(wchar_t (&v)[N]) { BindCol(2, v); }

    void OutCreateDate(TIMESTAMP_STRUCT& v) { BindCol(3, v); }

  private:
    int32_t _gold = {};
  };
};
