#include "pch.h"
#include "db_connection.h"

bool DbConnection::Connect(const SQLHENV env, const wchar_t* connection_string) {
  if (SQLAllocHandle(SQL_HANDLE_DBC, env, &_connection) != SQL_SUCCESS) {
    return false;
  }

  wchar_t buffer[MAX_PATH] = {};
  wcscpy_s(buffer, connection_string);

  wchar_t     result_string[MAX_PATH] = {};
  SQLSMALLINT result_length = 0;

  const SQLRETURN ret = SQLDriverConnectW(_connection,
                                          nullptr,
                                          buffer,
                                          _countof(buffer),
                                          result_string,
                                          _countof(result_string),
                                          &result_length,
                                          SQL_DRIVER_NOPROMPT);

  if (SQLAllocHandle(SQL_HANDLE_STMT, _connection, &_statement) != SQL_SUCCESS) {
    return false;
  }

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
    return true;
  }
}

void DbConnection::Clear() {
  if (_connection) {
    SQLFreeHandle(SQL_HANDLE_DBC, _connection);
    _connection = nullptr;
  }

  if (_statement) {
    SQLFreeHandle(SQL_HANDLE_STMT, _statement);
    _statement = nullptr;
  }
}

bool DbConnection::Execute(const wchar_t* query) const {
  const SQLRETURN ret = SQLExecDirectW(_statement, const_cast<SQLWCHAR*>(query), SQL_NTSL);

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
    return true;
  }

  HandleError(ret);
  return false;
}

bool DbConnection::Fetch() const {
  const SQLRETURN ret = SQLFetch(_statement);

  switch (ret) {
    case SQL_SUCCESS:
    case SQL_SUCCESS_WITH_INFO:
      return true;
    case SQL_NO_DATA:
      return false;
    case SQL_ERROR:
      HandleError(ret);
      return false;
    default:
      return true;
  }
}

int32_t DbConnection::GetRowCount() const {
  SQLLEN          count = 0;
  const SQLRETURN ret = SQLRowCount(_statement, &count);

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
    return static_cast<int32_t>(count);
  }

  return -1;
}

void DbConnection::GetResultColumnCount(SQLSMALLINT* count) const {
  SQLNumResultCols(_statement, count);
}

SQLRETURN DbConnection::GetMoreResult() const {
  return SQLMoreResults(_statement);
}

void DbConnection::Unbind() const {
  SQLFreeStmt(_statement, SQL_UNBIND);
  SQLFreeStmt(_statement, SQL_RESET_PARAMS);
  SQLFreeStmt(_statement, SQL_CLOSE);
}

bool DbConnection::BindParam(const int32_t param_index, bool* value, SQLLEN* index) {
  return BindParam(param_index, SQL_C_TINYINT, SQL_TINYINT, size32(bool), value, index);
}

bool DbConnection::BindParam(const int32_t param_index, float* value, SQLLEN* index) {
  return BindParam(param_index, SQL_C_FLOAT, SQL_REAL, 0, value, index);
}

bool DbConnection::BindParam(const int32_t param_index, double* value, SQLLEN* index) {
  return BindParam(param_index, SQL_C_DOUBLE, SQL_DOUBLE, 0, value, index);
}

bool DbConnection::BindParam(const int32_t param_index, int8_t* value, SQLLEN* index) {
  return BindParam(param_index, SQL_C_TINYINT, SQL_TINYINT, size32(int8_t), value, index);
}

bool DbConnection::BindParam(const int32_t param_index, int16_t* value, SQLLEN* index) {
  return BindParam(param_index, SQL_C_SHORT, SQL_SMALLINT, size32(int16_t), value, index);
}

bool DbConnection::BindParam(const int32_t param_index, int32_t* value, SQLLEN* index) {
  return BindParam(param_index, SQL_C_LONG, SQL_INTEGER, size32(int32_t), value, index);
}

bool DbConnection::BindParam(const int32_t param_index, int64_t* value, SQLLEN* index) {
  return BindParam(param_index, SQL_C_SBIGINT, SQL_BIGINT, size32(int64_t), value, index);
}

bool DbConnection::BindParam(const int32_t param_index, TIMESTAMP_STRUCT* value, SQLLEN* index) {
  return BindParam(param_index, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, size32(TIMESTAMP_STRUCT), value, index);
}

bool DbConnection::BindParam(const int32_t param_index, const wchar_t* str, SQLLEN* index) {
  const SQLULEN size = (wcslen(str) + 1) * sizeof(wchar_t);
  *index = SQL_NTSL;

  if (size > kWVarCharMax) {
    return BindParam(param_index, SQL_C_WCHAR, SQL_WLONGVARCHAR, size, (SQLPOINTER)str, index);
  }

  return BindParam(param_index, SQL_C_WCHAR, SQL_WVARCHAR, size, (SQLPOINTER)str, index);
}

bool DbConnection::BindParam(const int32_t param_index, const std::byte* data, int32_t size, SQLLEN* index) {
  if (data == nullptr) {
    *index = SQL_NULL_DATA;
    size = 1;
  } else {
    *index = size;
  }

  if (size > kBinaryMax) {
    return BindParam(param_index, SQL_C_BINARY, SQL_LONGVARBINARY, size, const_cast<std::byte*>(data), index);
  }

  return BindParam(param_index, SQL_C_BINARY, SQL_VARBINARY, size, const_cast<std::byte*>(data), index);
}

bool DbConnection::BindCol(const int32_t col_index, bool* value, SQLLEN* index) {
  return BindCol(col_index, SQL_C_TINYINT, size32(bool), value, index);
}

bool DbConnection::BindCol(const int32_t col_index, float* value, SQLLEN* index) {
  return BindCol(col_index, SQL_C_FLOAT, size32(float), value, index);
}

bool DbConnection::BindCol(const int32_t col_index, double* value, SQLLEN* index) {
  return BindCol(col_index, SQL_C_DOUBLE, size32(double), value, index);
}

bool DbConnection::BindCol(const int32_t col_index, int8_t* value, SQLLEN* index) {
  return BindCol(col_index, SQL_C_TINYINT, size32(int8_t), value, index);
}

bool DbConnection::BindCol(const int32_t col_index, int16_t* value, SQLLEN* index) {
  return BindCol(col_index, SQL_C_SHORT, size32(int16_t), value, index);
}

bool DbConnection::BindCol(const int32_t col_index, int32_t* value, SQLLEN* index) {
  return BindCol(col_index, SQL_C_LONG, size32(int32_t), value, index);
}

bool DbConnection::BindCol(const int32_t col_index, int64_t* value, SQLLEN* index) {
  return BindCol(col_index, SQL_C_SBIGINT, size32(int64_t), value, index);
}

bool DbConnection::BindCol(const int32_t col_index, TIMESTAMP_STRUCT* value, SQLLEN* index) {
  return BindCol(col_index, SQL_C_TYPE_TIMESTAMP, size32(TIMESTAMP_STRUCT), value, index);
}

bool DbConnection::BindCol(const int32_t col_index, wchar_t* str, const int32_t size, SQLLEN* index) {
  return BindCol(col_index, SQL_C_WCHAR, size, str, index);
}

bool DbConnection::BindCol(const int32_t col_index, std::byte* data, const int32_t size, SQLLEN* index) {
  return BindCol(col_index, SQL_C_BINARY, size, data, index);
}

bool DbConnection::BindParam(const SQLUSMALLINT param_index, const SQLSMALLINT c_type, const SQLSMALLINT sql_type, const SQLULEN len, const SQLPOINTER ptr, SQLLEN* index) const {
  const SQLRETURN ret = SQLBindParameter(_statement,
                                         param_index,
                                         SQL_PARAM_INPUT,
                                         c_type,
                                         sql_type,
                                         len,
                                         0,
                                         ptr,
                                         0,
                                         index);

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
    return true;
  }

  HandleError(ret);
  return false;
}

bool DbConnection::BindCol(const SQLUSMALLINT col_index, const SQLSMALLINT c_type, const SQLULEN len, const SQLPOINTER value, SQLLEN* index) const {
  const SQLRETURN ret = SQLBindCol(_statement, col_index, c_type, value, len, index);

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
    return true;
  }

  HandleError(ret);
  return false;
}

void DbConnection::HandleError(const SQLRETURN ret) const {
  if (ret == SQL_SUCCESS) {
    return;
  }

  SQLSMALLINT index = 1;
  SQLWCHAR    state[MAX_PATH] = {};
  SQLINTEGER  native_error = 0;
  SQLWCHAR    message[MAX_PATH] = {};
  SQLSMALLINT message_length = 0;

  while (true) {
    const SQLRETURN error_ret = SQLGetDiagRecW(SQL_HANDLE_STMT,
                                               _statement,
                                               index,
                                               state,
                                               &native_error,
                                               message,
                                               _countof(message),
                                               &message_length);

    if (error_ret == SQL_NO_DATA) {
      break;
    }

    if (error_ret != SQL_SUCCESS && error_ret != SQL_SUCCESS_WITH_INFO) {
      break;
    }

    std::wcout << message << std::endl;
    index++;
  }
}
