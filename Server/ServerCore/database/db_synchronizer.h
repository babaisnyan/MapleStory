#pragma once
#include "db_connection.h"
#include "db_model.h"

class DbSynchronizer {
  enum {
    kProcedureMaxLen = 10000
  };

  enum UpdateStep : uint8_t {
    kDropIndex,
    kAlterColumn,
    kAddColumn,
    kCreateTable,
    kDefaultConstraint,
    kCreateIndex,
    kDropColumn,
    kDropTable,
    kStoredProcedure,
    kMax
  };

  enum ColumnFlag : uint8_t {
    kType     = 1 << 0,
    kNullable = 1 << 1,
    kIdentity = 1 << 2,
    kDefault  = 1 << 3,
    kLength   = 1 << 4,
  };

public:
  explicit DbSynchronizer(DbConnection& conn) : _connection(conn) {}
  ~DbSynchronizer() = default;

  bool Synchronize(const wchar_t* path);

private:
  void ParseXmlDb(const wchar_t* path);
  bool GatherDbTables();
  bool GatherDbIndexes();
  bool GatherDbStoredProcedures();

  void CompareDbModel();
  void CompareTables(const db_model::TableRef& db_table, const db_model::TableRef& xml_table);
  void CompareColumns(const db_model::TableRef& db_table, const db_model::ColumnRef& db_column, const db_model::ColumnRef& xml_column);
  void CompareStoredProcedures();

  void ExecuteUpdateQueries();

private:
  DbConnection& _connection;

  Vector<db_model::TableRef>     _xml_tables;
  Vector<db_model::ProcedureRef> _xml_procedures;
  Set<String>                    _xml_removed_tables;

  Vector<db_model::TableRef>     _db_tables;
  Vector<db_model::ProcedureRef> _db_procedures;

private:
  Set<String>    _dependent_indexes;
  Vector<String> _update_queries[kMax];
};
