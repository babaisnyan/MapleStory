#pragma once
#include "db_connection.h"

template <int32_t C>
struct FullBits {
  static constexpr uint64_t value = 1 << (C - 1) | FullBits<C - 1>::value;
};

template <>
struct FullBits<1> {
  static constexpr uint64_t value = 1;
};

template <>
struct FullBits<0> {
  static constexpr uint64_t value = 0;
};

template <int32_t ParamCount, int32_t ColumnCount>
class DbBind {
public:
  DbBind(DbConnection& connection, const wchar_t* query) : _connection(connection), _query(query) {
    _connection.Unbind();
  }

  bool Validate() { return _param_flag == FullBits<ParamCount>::value && _col_flag == FullBits<ColumnCount>::value; }

  bool Execute() {
    ASSERT_CRASH(Validate());
    return _connection.Execute(_query);
  }

  bool Fetch() { return _connection.Fetch(); }

public:
  template <typename T>
  void BindParam(int32_t param_index, T& value) {
    _connection.BindParam(param_index + 1, &value, &_param_index[param_index]);
    _param_flag |= (1 << param_index);
  }

  void BindParam(int32_t param_index, wchar_t* str) {
    _connection.BindParam(param_index + 1, str, &_param_index[param_index]);
    _param_flag |= (1 << param_index);
  }

  template <typename T, int32_t N>
  void BindParam(int32_t param_index, const T (&data)[N]) {
    _connection.BindParam(param_index + 1, reinterpret_cast<const std::byte*>(data), size32(T) * N, &_param_index[param_index]);
    _param_flag |= (1 << param_index);
  }

  template <typename T>
  void BindParam(int32_t param_index, T* value, const int32_t count) {
    _connection.BindParam(param_index + 1, reinterpret_cast<const std::byte*>(value), size32(T) * count, &_param_index[param_index]);
    _param_flag |= (1 << param_index);
  }

  template <typename T>
  void BindCol(int32_t col_index, T& value) {
    _connection.BindCol(col_index + 1, &value, &_col_index[col_index]);
    _col_flag |= 1 << col_index;
  }

  template <int32_t N>
  void BindCol(int32_t col_index, wchar_t (&data)[N]) {
    _connection.BindCol(col_index + 1, data, N - 1, &_col_index[col_index]);
    _col_flag |= 1 << col_index;
  }

  void BindCol(int32_t col_index, wchar_t* value, const int32_t len) {
    _connection.BindCol(col_index + 1, value, len - 1, &_col_index[col_index]);
    _col_flag |= 1 << col_index;
  }

  template <typename T, int32_t N>
  void BindCol(int32_t col_index, T (&data)[N]) {
    _connection.BindCol(col_index + 1, data, size32(T) * N, &_col_index[col_index]);
    _col_flag |= 1 << col_index;
  }

protected
:
  DbConnection&  _connection;
  const wchar_t* _query = nullptr;
  SQLLEN         _param_index[ParamCount > 0 ? ParamCount : 1] = {};
  SQLLEN         _col_index[ColumnCount > 0 ? ColumnCount : 1] = {};
  uint64_t       _param_flag = 0;
  uint64_t       _col_flag = 0;
};
