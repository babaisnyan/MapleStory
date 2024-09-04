#pragma once

#include "db_connection.h"

class DbConnectionPool {
public:
  static DbConnectionPool& GetInstance() {
    static DbConnectionPool instance;
    return instance;
  }

public:
  DbConnectionPool() = default;
  ~DbConnectionPool();

  bool Connect(int32_t connection_count, const wchar_t* connection_string);
  void Clear();

  DbConnection* GetConnection();
  void          ReleaseConnection(DbConnection* connection);

private:
  USE_LOCK;
  SQLHENV               _env = nullptr;
  Vector<DbConnection*> _connections;
};
