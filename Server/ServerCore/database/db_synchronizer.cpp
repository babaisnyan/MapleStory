#include "pch.h"
#include "db_synchronizer.h"

#include <ranges>
#include <regex>

#include "db_bind.h"
#include "db_model.h"
#include "log/console_log.h"
#include "memory/memory.h"
#include "utils/xml/xml_parser.h"

namespace sp {
  const wchar_t* tables_and_columns =
    L"	SELECT c.object_id, t.name AS tableName, c.name AS columnName, c.column_id, c.user_type_id, c.max_length,"
    "		c.is_nullable, c.is_identity, CAST(ic.seed_value AS BIGINT) AS seedValue, CAST(ic.increment_value AS BIGINT) AS incValue,"
    "		c.default_object_id, dc.definition as defaultDefinition, dc.name as defaultConstraintName"
    "	FROM sys.columns AS c"
    "	JOIN sys.tables AS t"
    "		ON c.object_id = t.object_id"
    "	LEFT JOIN sys.default_constraints AS dc"
    "		ON c.default_object_id = dc.object_id"
    "	LEFT JOIN sys.identity_columns AS ic"
    "		ON c.object_id = ic.object_id AND c.column_id = ic.column_id"
    "	WHERE t.type = 'U'"
    "	ORDER BY object_id ASC, column_id ASC;";

  class GetDbTables : public DbBind<0, 13> {
  public:
    explicit GetDbTables(DbConnection& conn) : DbBind(conn, tables_and_columns) {}

    void OutObjectId(int32_t& value) { BindCol(0, value); }

    template <int32_t N>
    void OutTableName(wchar_t (&value)[N]) { BindCol(1, value); }

    template <int32_t N>
    void OutColumnName(wchar_t (&value)[N]) { BindCol(2, value); }

    void OutColumnId(int32_t& value) { BindCol(3, value); }
    void OutUserType(int32_t& value) { BindCol(4, value); }
    void OutMaxLength(int32_t& value) { BindCol(5, value); }
    void OutIsNullable(bool& value) { BindCol(6, value); }
    void OutIsIdentity(bool& value) { BindCol(7, value); }
    void OutSeedValue(int64_t& value) { BindCol(8, value); }
    void OutIncrementValue(int64_t& value) { BindCol(9, value); }
    void OutDefaultObjectId(int32_t& value) { BindCol(10, value); }

    template <int32_t N>
    void OutDefaultDefinition(wchar_t (&value)[N]) { BindCol(11, value); }

    template <int32_t N>
    void OutDefaultConstraintName(wchar_t (&value)[N]) { BindCol(12, value); }
  };

  const wchar_t* indexes =
    L"	SELECT i.object_id, i.name as indexName, i.index_id, i.type, i.is_primary_key,"
    "		i.is_unique_constraint, ic.column_id, COL_NAME(ic.object_id, ic.column_id) as columnName"
    "	FROM sys.indexes AS i"
    "	JOIN sys.index_columns AS ic"
    "		ON i.object_id = ic.object_id AND i.index_id = ic.index_id"
    "	WHERE i.type > 0 AND i.object_id IN(SELECT object_id FROM sys.tables WHERE type = 'U')"
    "	ORDER BY i.object_id ASC, i.index_id ASC;";

  class GetDbIndexes : public DbBind<0, 8> {
  public:
    explicit GetDbIndexes(DbConnection& conn) : DbBind(conn, indexes) {}

    void OutObjectId(int32_t& value) { BindCol(0, value); }

    template <int32_t N>
    void OutIndexName(wchar_t (&value)[N]) { BindCol(1, value); }

    void OutIndexId(int32_t& value) { BindCol(2, value); }
    void OutIndexType(int32_t& value) { BindCol(3, value); }
    void OutIsPrimaryKey(bool& value) { BindCol(4, value); }
    void OutIsUniqueConstraint(bool& value) { BindCol(5, value); }
    void OutColumnId(int32_t& value) { BindCol(6, value); }

    template <int32_t N>
    void OutColumnName(wchar_t (&value)[N]) { BindCol(7, value); }
  };

  const wchar_t* stored_procedures = L"	SELECT name, OBJECT_DEFINITION(object_id) AS body FROM sys.procedures;";

  class GetDbStoredProcedures : public DbBind<0, 2> {
  public:
    explicit GetDbStoredProcedures(DbConnection& conn) : DbBind(conn, stored_procedures) {}

    template <int32_t N>
    void OutName(wchar_t (&value)[N]) { BindCol(0, value); }

    void OutBody(wchar_t* value, const int32_t len) { BindCol(1, value, len); }
  };
}


bool DbSynchronizer::Synchronize(const wchar_t* path) {
  ParseXmlDb(path);

  GatherDbTables();
  GatherDbIndexes();
  GatherDbStoredProcedures();

  CompareDbModel();
  ExecuteUpdateQueries();

  return true;
}

void DbSynchronizer::ParseXmlDb(const wchar_t* path) {
  XmlNode   root;
  XmlParser parser;
  ASSERT_CRASH(parser.ParseFromFile(path, root));

  Vector<XmlNode> tables = root.FindChildren(L"Table");

  for (XmlNode& table : tables) {
    db_model::TableRef t = MakeShared<db_model::Table>();
    t->_name = table.GetStringAttr(L"name");

    Vector<XmlNode> columns = table.FindChildren(L"Column");

    for (XmlNode& column : columns) {
      db_model::ColumnRef c = MakeShared<db_model::Column>();
      c->_name = column.GetStringAttr(L"name");
      c->_type_text = column.GetStringAttr(L"type");
      c->_type = db_model::Helpers::StringToDataType(c->_type_text.c_str(), c->_max_length);
      ASSERT_CRASH(c->_type != db_model::DataType::kNone);
      c->_nullable = !column.GetBoolAttr(L"notnull", false);

      const wchar_t* identity_str = column.GetStringAttr(L"identity");

      if (wcslen(identity_str) > 0) {
        std::wregex  pt(L"(\\d+),(\\d+)");
        std::wcmatch match;
        ASSERT_CRASH(std::regex_match(identity_str, match, pt));
        c->_identity = true;
        c->_seed_value = _wtoi(match[1].str().c_str());
        c->_increment_value = _wtoi(match[2].str().c_str());
      }

      c->_default = column.GetStringAttr(L"default");
      t->_columns.push_back(c);
    }

    Vector<XmlNode> indexes = table.FindChildren(L"Index");

    for (XmlNode& index : indexes) {
      db_model::IndexRef i = MakeShared<db_model::Index>();
      const wchar_t*     type_str = index.GetStringAttr(L"type");
      if (_wcsicmp(type_str, L"clustered") == 0) i->_type = db_model::IndexType::kClustered;
      else if (_wcsicmp(type_str, L"nonclustered") == 0) i->_type = db_model::IndexType::kNonClustered;
      else CRASH("Invalid Index Type");

      i->_primary_key = index.FindChild(L"PrimaryKey").IsValid();
      i->_unique_constraint = index.FindChild(L"UniqueKey").IsValid();

      Vector<XmlNode> columns = index.FindChildren(L"Column");
      for (XmlNode& column : columns) {
        const wchar_t*      name_str = column.GetStringAttr(L"name");
        db_model::ColumnRef c = t->FindColumn(name_str);
        ASSERT_CRASH(c != nullptr);
        i->_columns.push_back(c);
      }

      t->_indexes.push_back(i);
    }

    _xml_tables.push_back(t);
  }

  Vector<XmlNode> procedures = root.FindChildren(L"Procedure");
  for (XmlNode& procedure : procedures) {
    db_model::ProcedureRef p = MakeShared<db_model::Procedure>();
    p->_name = procedure.GetStringAttr(L"name");
    p->_body = procedure.FindChild(L"Body").GetStringValue();

    Vector<XmlNode> params = procedure.FindChildren(L"Param");
    for (XmlNode& param_node : params) {
      db_model::Param param;
      param.name = param_node.GetStringAttr(L"name");
      param.type = param_node.GetStringAttr(L"type");
      p->_parameters.push_back(param);
    }

    _xml_procedures.push_back(p);
  }

  Vector<XmlNode> removed_tables = root.FindChildren(L"RemovedTable");
  for (XmlNode& removed_table : removed_tables) {
    _xml_removed_tables.insert(removed_table.GetStringAttr(L"name"));
  }
}

bool DbSynchronizer::GatherDbTables() {
  int32_t object_id;
  wchar_t table_name[101] = {};
  wchar_t column_name[101] = {};
  int32_t column_id;
  int32_t user_type_id;
  int32_t max_length;
  bool    is_nullable;
  bool    is_identity;
  int64_t seed_value;
  int64_t inc_value;
  int32_t default_object_id;
  wchar_t default_definition[101] = {};
  wchar_t default_constraint_name[101] = {};

  sp::GetDbTables get_db_tables(_connection);

  get_db_tables.OutObjectId(object_id);
  get_db_tables.OutTableName(table_name);
  get_db_tables.OutColumnName(column_name);
  get_db_tables.OutColumnId(column_id);
  get_db_tables.OutUserType(user_type_id);
  get_db_tables.OutMaxLength(max_length);
  get_db_tables.OutIsNullable(is_nullable);
  get_db_tables.OutIsIdentity(is_identity);
  get_db_tables.OutSeedValue(seed_value);
  get_db_tables.OutIncrementValue(inc_value);
  get_db_tables.OutDefaultObjectId(default_object_id);
  get_db_tables.OutDefaultDefinition(default_definition);
  get_db_tables.OutDefaultConstraintName(default_constraint_name);

  if (!get_db_tables.Execute()) return false;

  while (get_db_tables.Fetch()) {
    db_model::TableRef table;

    auto find_table = std::ranges::find_if(_db_tables, [=](const db_model::TableRef& table) { return table->_object_id == object_id; });
    if (find_table == _db_tables.end()) {
      table = MakeShared<db_model::Table>();
      table->_object_id = object_id;
      table->_name = table_name;
      _db_tables.push_back(table);
    } else {
      table = *find_table;
    }

    db_model::ColumnRef column = MakeShared<db_model::Column>();
    {
      column->_name = column_name;
      column->_column_id = column_id;
      column->_type = static_cast<db_model::DataType>(user_type_id);
      column->_type_text = db_model::Helpers::DataTypeToString(column->_type);
      column->_max_length = (column->_type == db_model::DataType::kNVarChar ? max_length / 2 : max_length);
      column->_nullable = is_nullable;
      column->_identity = is_identity;
      column->_seed_value = (is_identity ? seed_value : 0);
      column->_increment_value = (is_identity ? inc_value : 0);

      if (default_object_id > 0) {
        column->_default = default_definition;
        const uint64_t p = column->_default.find_first_not_of('(');
        column->_default = column->_default.substr(p, column->_default.size() - p * 2);
        column->_default_constraint_name = default_constraint_name;
      }
    }

    table->_columns.push_back(column);
  }

  return true;
}

bool DbSynchronizer::GatherDbIndexes() {
  int32_t object_id;
  wchar_t index_name[101] = {};
  int32_t index_id;
  int32_t index_type;
  bool    is_primary_key;
  bool    is_unique_constraint;
  int32_t column_id;
  wchar_t column_name[101] = {};

  sp::GetDbIndexes get_db_indexes(_connection);
  get_db_indexes.OutObjectId(object_id);
  get_db_indexes.OutIndexName(index_name);
  get_db_indexes.OutIndexId(index_id);
  get_db_indexes.OutIndexType(index_type);
  get_db_indexes.OutIsPrimaryKey(is_primary_key);
  get_db_indexes.OutIsUniqueConstraint(is_unique_constraint);
  get_db_indexes.OutColumnId(column_id);
  get_db_indexes.OutColumnName(column_name);

  if (!get_db_indexes.Execute()) return false;

  while (get_db_indexes.Fetch()) {
    auto find_table = std::ranges::find_if(_db_tables, [=](const db_model::TableRef& table) { return table->_object_id == object_id; });
    ASSERT_CRASH(find_table != _db_tables.end());
    Vector<db_model::IndexRef>& indexes = (*find_table)->_indexes;
    auto                        find_index = std::ranges::find_if(indexes, [index_id](const db_model::IndexRef& index) { return index->_index_id == index_id; });
    if (find_index == indexes.end()) {
      db_model::IndexRef index = MakeShared<db_model::Index>();
      {
        index->_name = index_name;
        index->_index_id = index_id;
        index->_type = static_cast<db_model::IndexType>(index_type);
        index->_primary_key = is_primary_key;
        index->_unique_constraint = is_unique_constraint;
      }
      indexes.push_back(index);
      find_index = indexes.end() - 1;
    }

    Vector<db_model::ColumnRef>& columns = (*find_table)->_columns;
    auto                         find_column = std::ranges::find_if(columns, [column_id](const db_model::ColumnRef& column) { return column->_column_id == column_id; });
    ASSERT_CRASH(find_column != columns.end());
    (*find_index)->_columns.push_back(*find_column);
  }

  return true;
}

bool DbSynchronizer::GatherDbStoredProcedures() {
  wchar_t         name[101] = {};
  Vector<wchar_t> body(kProcedureMaxLen);

  sp::GetDbStoredProcedures get_db_stored_procedures(_connection);
  get_db_stored_procedures.OutName(name);
  get_db_stored_procedures.OutBody(body.data(), kProcedureMaxLen);

  if (!get_db_stored_procedures.Execute()) return false;

  while (get_db_stored_procedures.Fetch()) {
    db_model::ProcedureRef proc = MakeShared<db_model::Procedure>();
    {
      proc->_name = name;
      proc->_full_body = String(body.begin(), std::ranges::find(body, 0));
    }
    _db_procedures.push_back(proc);
  }

  return true;
}

void DbSynchronizer::CompareDbModel() {
  _dependent_indexes.clear();
  for (Vector<String>& queries : _update_queries) queries.clear();

  Map<String, db_model::TableRef> xml_table_map;
  for (const db_model::TableRef& xml_table : _xml_tables) xml_table_map[xml_table->_name] = xml_table;

  for (const db_model::TableRef& db_table : _db_tables) {
    auto find_table = xml_table_map.find(db_table->_name);
    if (find_table != xml_table_map.end()) {
      const db_model::TableRef xml_table = find_table->second;
      CompareTables(db_table, xml_table);
      xml_table_map.erase(find_table);
    } else {
      if (_xml_removed_tables.contains(db_table->_name)) {
        ConsoleLog::GetInstance().WriteStdOut(Color::kYellow, L"Removing Table : [dbo].[%s]\n", db_table->_name.c_str());
        _update_queries[kDropTable].push_back(db_model::Helpers::Format(L"DROP TABLE [dbo].[%s]", db_table->_name.c_str()));
      }
    }
  }

  for (auto& val : xml_table_map | std::views::values) {
    const db_model::TableRef& xml_table = val;

    String        columns_str;
    const int32_t size = static_cast<int32_t>(xml_table->_columns.size());
    for (int32_t i = 0; i < size; i++) {
      if (i != 0) columns_str += L",";
      columns_str += L"\n\t";
      columns_str += xml_table->_columns[i]->CreateText();
    }

    ConsoleLog::GetInstance().WriteStdOut(Color::kYellow, L"Creating Table : [dbo].[%s]\n", xml_table->_name.c_str());
    _update_queries[kCreateTable].push_back(db_model::Helpers::Format(L"CREATE TABLE [dbo].[%s] (%s)", xml_table->_name.c_str(), columns_str.c_str()));

    for (const db_model::ColumnRef& xml_column : xml_table->_columns) {
      if (xml_column->_default.empty()) continue;

      _update_queries[kDefaultConstraint].push_back(db_model::Helpers::Format(L"ALTER TABLE [dbo].[%s] ADD CONSTRAINT [%s] DEFAULT (%s) FOR [%s]",
                                                                              xml_table->_name.c_str(),
                                                                              db_model::Helpers::Format(L"DF_%s_%s", xml_table->_name.c_str(), xml_column->_name.c_str()).c_str(),
                                                                              xml_column->_default.c_str(),
                                                                              xml_column->_name.c_str()));
    }

    for (const db_model::IndexRef& xml_index : xml_table->_indexes) {
      ConsoleLog::GetInstance().WriteStdOut(Color::kYellow, L"Creating Index : [%s] %s %s [%s]\n", xml_table->_name.c_str(), xml_index->GetKeyText().c_str(), xml_index->GetTypeText().c_str(), xml_index->GetUniqueName().c_str());
      if (xml_index->_primary_key || xml_index->_unique_constraint) {
        _update_queries[kCreateIndex].push_back(db_model::Helpers::Format(L"ALTER TABLE [dbo].[%s] ADD CONSTRAINT [%s] %s %s (%s)",
                                                                          xml_table->_name.c_str(),
                                                                          xml_index->CreateName(xml_table->_name).c_str(),
                                                                          xml_index->GetKeyText().c_str(),
                                                                          xml_index->GetTypeText().c_str(),
                                                                          xml_index->CreateColumnsText().c_str()));
      } else {
        _update_queries[kCreateIndex].push_back(db_model::Helpers::Format(L"CREATE %s INDEX [%s] ON [dbo].[%s] (%s)",
                                                                          xml_index->GetTypeText().c_str(),
                                                                          xml_index->CreateName(xml_table->_name).c_str(),
                                                                          xml_table->_name.c_str(),
                                                                          xml_index->CreateColumnsText().c_str()));
      }
    }
  }

  CompareStoredProcedures();
}

void DbSynchronizer::ExecuteUpdateQueries() {
  for (int32_t step = 0; step < kMax; step++) {
    for (String& query : _update_queries[step]) {
      _connection.Unbind();
      ASSERT_CRASH(_connection.Execute(query.c_str()));
    }
  }
}

void DbSynchronizer::CompareTables(const db_model::TableRef& db_table, const db_model::TableRef& xml_table) {
  Map<String, db_model::ColumnRef> xml_column_map;
  for (const db_model::ColumnRef& xml_column : xml_table->_columns) xml_column_map[xml_column->_name] = xml_column;

  for (const db_model::ColumnRef& db_column : db_table->_columns) {
    auto find_column = xml_column_map.find(db_column->_name);
    if (find_column != xml_column_map.end()) {
      const db_model::ColumnRef& xml_column = find_column->second;
      CompareColumns(db_table, db_column, xml_column);
      xml_column_map.erase(find_column);
    } else {
      ConsoleLog::GetInstance().WriteStdOut(Color::kYellow, L"Dropping Column : [%s].[%s]\n", db_table->_name.c_str(), db_column->_name.c_str());
      if (!db_column->_default_constraint_name.empty()) _update_queries[kDropColumn].push_back(db_model::Helpers::Format(L"ALTER TABLE [dbo].[%s] DROP CONSTRAINT [%s]", db_table->_name.c_str(), db_column->_default_constraint_name.c_str()));

      _update_queries[kDropColumn].push_back(db_model::Helpers::Format(L"ALTER TABLE [dbo].[%s] DROP COLUMN [%s]", db_table->_name.c_str(), db_column->_name.c_str()));
    }
  }


  for (auto& val : xml_column_map | std::views::values) {
    db_model::ColumnRef& xml_column = val;
    db_model::Column     new_column = *xml_column;
    new_column._nullable = true;

    ConsoleLog::GetInstance().WriteStdOut(Color::kYellow, L"Adding Column : [%s].[%s]\n", db_table->_name.c_str(), xml_column->_name.c_str());
    _update_queries[kAddColumn].push_back(db_model::Helpers::Format(L"ALTER TABLE [dbo].[%s] ADD %s %s",
                                                                    db_table->_name.c_str(), xml_column->_name.c_str(), xml_column->_type_text.c_str()));

    if (!xml_column->_nullable && !xml_column->_default.empty()) {
      _update_queries[kAddColumn].push_back(db_model::Helpers::Format(L"SET NOCOUNT ON; UPDATE [dbo].[%s] SET [%s] = %s WHERE [%s] IS NULL",
                                                                      db_table->_name.c_str(), xml_column->_name.c_str(), xml_column->_default.c_str(), xml_column->_name.c_str()));
    }

    if (!xml_column->_nullable) {
      _update_queries[kAddColumn].push_back(db_model::Helpers::Format(L"ALTER TABLE [dbo].[%s] ALTER COLUMN %s",
                                                                      db_table->_name.c_str(), xml_column->CreateText().c_str()));
    }

    if (!xml_column->_default.empty()) {
      _update_queries[kAddColumn].push_back(db_model::Helpers::Format(L"ALTER TABLE [dbo].[%s] ADD CONSTRAINT [DF_%s_%s] DEFAULT (%s) FOR [%s]",
                                                                      db_table->_name.c_str(), db_table->_name.c_str(), xml_column->_name.c_str(), xml_column->_default.c_str(), xml_column->_name.c_str()));
    }
  }

  Map<String, db_model::IndexRef> xml_index_map;
  for (const db_model::IndexRef& xml_index : xml_table->_indexes) xml_index_map[xml_index->GetUniqueName()] = xml_index;

  for (const db_model::IndexRef& db_index : db_table->_indexes) {
    auto find_index = xml_index_map.find(db_index->GetUniqueName());
    if (find_index != xml_index_map.end() && !_dependent_indexes.contains(db_index->GetUniqueName())) {
      db_model::IndexRef xml_index = find_index->second;
      xml_index_map.erase(find_index);
    } else {
      ConsoleLog::GetInstance().WriteStdOut(Color::kYellow, L"Dropping Index : [%s] [%s] %s %s\n", db_table->_name.c_str(), db_index->_name.c_str(), db_index->GetKeyText().c_str(), db_index->GetTypeText().c_str());
      if (db_index->_primary_key || db_index->_unique_constraint) _update_queries[kDropIndex].push_back(db_model::Helpers::Format(L"ALTER TABLE [dbo].[%s] DROP CONSTRAINT [%s]", db_table->_name.c_str(), db_index->_name.c_str()));
      else _update_queries[kDropIndex].push_back(db_model::Helpers::Format(L"DROP INDEX [%s] ON [dbo].[%s]", db_index->_name.c_str(), db_table->_name.c_str()));
    }
  }

  for (const auto& val : xml_index_map | std::views::values) {
    const db_model::IndexRef xml_index = val;
    ConsoleLog::GetInstance().WriteStdOut(Color::kYellow, L"Creating Index : [%s] %s %s [%s]\n", db_table->_name.c_str(), xml_index->GetKeyText().c_str(), xml_index->GetTypeText().c_str(), xml_index->GetUniqueName().c_str());
    if (xml_index->_primary_key || xml_index->_unique_constraint) {
      _update_queries[kCreateIndex].push_back(db_model::Helpers::Format(L"ALTER TABLE [dbo].[%s] ADD CONSTRAINT [%s] %s %s (%s)",
                                                                        db_table->_name.c_str(), xml_index->CreateName(db_table->_name).c_str(), xml_index->GetKeyText().c_str(), xml_index->GetTypeText().c_str(), xml_index->CreateColumnsText().c_str()));
    } else {
      _update_queries[kCreateIndex].push_back(db_model::Helpers::Format(L"CREATE %s INDEX [%s] ON [dbo].[%s] (%s)",
                                                                        xml_index->GetTypeText(), xml_index->CreateName(db_table->_name).c_str(), db_table->_name.c_str(), xml_index->CreateColumnsText().c_str()));
    }
  }
}

void DbSynchronizer::CompareColumns(const db_model::TableRef& db_table, const db_model::ColumnRef& db_column, const db_model::ColumnRef& xml_column) {
  uint8_t flag = 0;

  if (db_column->_type != xml_column->_type) flag |= kType;
  if (db_column->_max_length != xml_column->_max_length && xml_column->_max_length > 0) flag |= kLength;
  if (db_column->_nullable != xml_column->_nullable) flag |= kNullable;
  if (db_column->_identity != xml_column->_identity || (db_column->_identity && db_column->_increment_value != xml_column->_increment_value)) flag |= kIdentity;
  if (db_column->_default != xml_column->_default) flag |= kDefault;

  if (flag) {
    ConsoleLog::GetInstance().WriteStdOut(Color::kYellow, L"Updating Column [%s] : (%s) -> (%s)\n", db_table->_name.c_str(), db_column->CreateText().c_str(), xml_column->CreateText().c_str());
  }

  if (flag & (kType | kLength | kNullable)) {
    for (db_model::IndexRef& dbIndex : db_table->_indexes) if (dbIndex->DependsOn(db_column->_name)) _dependent_indexes.insert(dbIndex->GetUniqueName());

    flag |= kDefault;
  }

  if (flag & kDefault) {
    if (!db_column->_default_constraint_name.empty()) {
      _update_queries[kAlterColumn].push_back(db_model::Helpers::Format(L"ALTER TABLE [dbo].[%s] DROP CONSTRAINT [%s]",
                                                                        db_table->_name.c_str(),
                                                                        db_column->_default_constraint_name.c_str()));
    }
  }

  db_model::Column newColumn = *db_column;
  newColumn._default = L"";
  newColumn._type = xml_column->_type;
  newColumn._max_length = xml_column->_max_length;
  newColumn._type_text = xml_column->_type_text;
  newColumn._seed_value = xml_column->_seed_value;
  newColumn._increment_value = xml_column->_increment_value;

  if (flag & (kType | kLength | kIdentity)) {
    _update_queries[kAlterColumn].push_back(db_model::Helpers::Format(
                                                                      L"ALTER TABLE [dbo].[%s] ALTER COLUMN %s",
                                                                      db_table->_name.c_str(),
                                                                      newColumn.CreateText().c_str()));
  }

  newColumn._nullable = xml_column->_nullable;
  if (flag & kNullable) {
    if (!xml_column->_default.empty()) {
      _update_queries[kAlterColumn].push_back(db_model::Helpers::Format(L"SET NOCOUNT ON; UPDATE [dbo].[%s] SET [%s] = %s WHERE [%s] IS NULL",
                                                                        db_table->_name.c_str(),
                                                                        xml_column->_name.c_str(),
                                                                        xml_column->_name.c_str(),
                                                                        xml_column->_name.c_str()));
    }

    _update_queries[kAlterColumn].push_back(db_model::Helpers::Format(L"ALTER TABLE [dbo].[%s] ALTER COLUMN %s",
                                                                      db_table->_name.c_str(),
                                                                      newColumn.CreateText().c_str()));
  }

  if (flag & kDefault) {
    if (!db_column->_default_constraint_name.empty()) {
      _update_queries[kAlterColumn].push_back(db_model::Helpers::Format(L"ALTER TABLE [dbo].[%s] ADD CONSTRAINT [%s] DEFAULT (%s) FOR [%s]",
                                                                        db_table->_name.c_str(),
                                                                        db_model::Helpers::Format(L"DF_%s_%s", db_table->_name.c_str(), db_column->_name.c_str()).c_str(),
                                                                        db_column->_default.c_str(), db_column->_name.c_str()));
    }
  }
}

void DbSynchronizer::CompareStoredProcedures() {
  Map<String, db_model::ProcedureRef> xml_procedures_map;
  for (const db_model::ProcedureRef& xml_procedure : _xml_procedures) xml_procedures_map[xml_procedure->_name] = xml_procedure;

  for (const db_model::ProcedureRef& db_procedure : _db_procedures) {
    auto find_procedure = xml_procedures_map.find(db_procedure->_name);
    if (find_procedure != xml_procedures_map.end()) {
      const db_model::ProcedureRef xml_procedure = find_procedure->second;
      String                       xml_body = xml_procedure->GenerateCreateQuery();
      if (db_model::Helpers::RemoveWhiteSpace(db_procedure->_full_body) != db_model::Helpers::RemoveWhiteSpace(xml_body)) {
        ConsoleLog::GetInstance().WriteStdOut(Color::kYellow, L"Updating Procedure : %s\n", db_procedure->_name.c_str());
        _update_queries[kStoredProcedure].push_back(xml_procedure->GenerateAlterQuery());
      }
      xml_procedures_map.erase(find_procedure);
    }
  }

  for (auto& map_it : xml_procedures_map) {
    ConsoleLog::GetInstance().WriteStdOut(Color::kYellow, L"Updating Procedure : %s\n", map_it.first.c_str());
    _update_queries[kStoredProcedure].push_back(map_it.second->GenerateCreateQuery());
  }
}
