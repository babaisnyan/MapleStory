#pragma once
#include <sql.h>
#include <sqlext.h>

enum {
  kWVarCharMax = 4000,
  kBinaryMax   = 8000
};

class DbConnection {
public:
  bool Connect(SQLHENV env, const wchar_t* connection_string);
  void Clear();

  bool      Execute(const wchar_t* query) const;
  bool      Fetch() const;
  int32_t   GetRowCount() const;
  void      GetResultColumnCount(SQLSMALLINT* count) const;
  SQLRETURN GetMoreResult() const;
  void      Unbind() const;

public:
  bool BindParam(int32_t param_index, bool* value, SQLLEN* index);
  bool BindParam(int32_t param_index, float* value, SQLLEN* index);
  bool BindParam(int32_t param_index, double* value, SQLLEN* index);
  bool BindParam(int32_t param_index, int8_t* value, SQLLEN* index);
  bool BindParam(int32_t param_index, int16_t* value, SQLLEN* index);
  bool BindParam(int32_t param_index, int32_t* value, SQLLEN* index);
  bool BindParam(int32_t param_index, int64_t* value, SQLLEN* index);
  bool BindParam(int32_t param_index, TIMESTAMP_STRUCT* value, SQLLEN* index);
  bool BindParam(int32_t param_index, const wchar_t* str, SQLLEN* index);
  bool BindParam(int32_t param_index, const std::byte* data, int32_t size, SQLLEN* index);

  bool BindCol(int32_t col_index, bool* value, SQLLEN* index);
  bool BindCol(int32_t col_index, float* value, SQLLEN* index);
  bool BindCol(int32_t col_index, double* value, SQLLEN* index);
  bool BindCol(int32_t col_index, int8_t* value, SQLLEN* index);
  bool BindCol(int32_t col_index, int16_t* value, SQLLEN* index);
  bool BindCol(int32_t col_index, int32_t* value, SQLLEN* index);
  bool BindCol(int32_t col_index, int64_t* value, SQLLEN* index);
  bool BindCol(int32_t col_index, TIMESTAMP_STRUCT* value, SQLLEN* index);
  bool BindCol(int32_t col_index, wchar_t* str, int32_t size, SQLLEN* index);
  bool BindCol(int32_t col_index, std::byte* data, int32_t size, SQLLEN* index);

private:
  bool BindParam(SQLUSMALLINT param_index, SQLSMALLINT c_type, SQLSMALLINT sql_type, SQLULEN len, SQLPOINTER ptr, SQLLEN* index) const;
  bool BindCol(SQLUSMALLINT col_index, SQLSMALLINT c_type, SQLULEN len, SQLPOINTER value, SQLLEN* index) const;
  void HandleError(SQLRETURN ret) const;

private:
  SQLHDBC  _connection = nullptr;
  SQLHSTMT _statement = nullptr;
};
