#include "pch.h"
#include "db_connection_pool.h"

#include "memory/memory.h"

DbConnectionPool::~DbConnectionPool() {
  Clear();
}

bool DbConnectionPool::Connect(const int32_t connection_count, const wchar_t* connection_string) {
  WRITE_LOCK;

  if (SQLAllocHandle(SQL_HANDLE_ENV, nullptr, &_env) != SQL_SUCCESS) {
    return false;
  }

  if (SQLSetEnvAttr(_env, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0) != SQL_SUCCESS) {
    return false;
  }

  for (int32_t i = 0; i < connection_count; i++) {
    DbConnection* connection = MsNew<DbConnection>();

    if (!connection->Connect(_env, connection_string)) {
      return false;
    }

    _connections.push_back(connection);
  }

  return true;
}

void DbConnectionPool::Clear() {
  WRITE_LOCK;

  if (_env) {
    SQLFreeHandle(SQL_HANDLE_ENV, _env);
    _env = nullptr;
  }

  for (DbConnection* connection : _connections) {
    MsDelete(connection);
  }

  _connections.clear();
}

DbConnection* DbConnectionPool::GetConnection() {
  WRITE_LOCK;

  if (_connections.empty()) {
    return nullptr;
  }

  DbConnection* connection = _connections.back();
  _connections.pop_back();
  return connection;
}

void DbConnectionPool::ReleaseConnection(DbConnection* connection) {
  WRITE_LOCK;
  _connections.push_back(connection);
}
