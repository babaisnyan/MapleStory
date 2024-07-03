#pragma once

namespace db_model {
  DECLARE_SHARED_PTR(Column);
  DECLARE_SHARED_PTR(Index);
  DECLARE_SHARED_PTR(Table);
  DECLARE_SHARED_PTR(Procedure);

  enum class DataType {
    kNone      = 0,
    kTinyInt   = 48,
    kSmallInt  = 52,
    kInt       = 56,
    kReal      = 59,
    kDateTime  = 61,
    kFloat     = 62,
    kBit       = 104,
    kNumeric   = 108,
    kBigInt    = 127,
    kVarBinary = 165,
    kVarchar   = 167,
    kBinary    = 173,
    kNVarChar  = 231,
  };

  class Column {
  public:
    String CreateText();

  public:
    String   _name;
    int32_t  _column_id = 0;
    DataType _type = DataType::kNone;
    String   _type_text;
    int32_t  _max_length = 0;
    bool     _nullable = false;
    bool     _identity = false;
    int64_t  _seed_value = 0;
    int64_t  _increment_value = 0;
    String   _default;
    String   _default_constraint_name;
  };

  enum class IndexType {
    kClustered    = 1,
    kNonClustered = 2
  };

  class Index {
  public:
    String GetUniqueName();
    String CreateName(const String& table_name);
    String GetTypeText();
    String GetKeyText();
    String CreateColumnsText();
    bool   DependsOn(const String& column_name);

  public:
    String            _name;
    int32_t           _index_id = 0;
    IndexType         _type = IndexType::kNonClustered;
    bool              _primary_key = false;
    bool              _unique_constraint = false;
    Vector<ColumnRef> _columns;
  };

  class Table {
  public:
    ColumnRef FindColumn(const String& column_name);

  public:
    int32_t           _object_id = 0;
    String            _name;
    Vector<ColumnRef> _columns;
    Vector<IndexRef>  _indexes;
  };

  struct Param {
    String name;
    String type;
  };

  class Procedure {
  public:
    String GenerateCreateQuery();
    String GenerateAlterQuery();
    String GenerateParamString();

  public:
    String        _name;
    String        _full_body;
    String        _body;
    Vector<Param> _parameters;
  };

  class Helpers {
  public:
    static String   Format(const wchar_t* format, ...);
    static String   DataTypeToString(DataType type);
    static String   RemoveWhiteSpace(const String& str);
    static DataType StringToDataType(const wchar_t* str, int32_t& max_len);
  };
}
