#include "pch.h"
#include "db_model.h"

#include <regex>

using namespace db_model;

String Column::CreateText() {
  return db_model::Helpers::Format(L"[%s] %s %s %s",
                                   _name.c_str(),
                                   _type_text.c_str(),
                                   _nullable ? L"NULL" : L"NOT NULL",
                                   _identity ? db_model::Helpers::Format(L"IDENTITY(%d, %d)", _seed_value, _increment_value).c_str() : L"");
}

String Index::GetUniqueName() {
  String ret;
  ret += _primary_key ? L"PK " : L" ";
  ret += _unique_constraint ? L"UK " : L" ";
  ret += (_type == IndexType::kClustered ? L"C " : L"NC ");

  for (const ColumnRef& column : _columns) {
    ret += L"*";
    ret += column->_name;
    ret += L" ";
  }

  return ret;
}

String Index::CreateName(const String& table_name) {
  String ret = L"IX_" + table_name;

  for (const ColumnRef& column : _columns) {
    ret += L"_";
    ret += column->_name;
  }

  return ret;
}

String Index::GetTypeText() {
  return (_type == IndexType::kClustered ? L"CLUSTERED" : L"NONCLUSTERED");
}

String Index::GetKeyText() {
  if (_primary_key) return L"PRIMARY KEY";
  if (_unique_constraint) return L"UNIQUE";

  return L"";
}

String Index::CreateColumnsText() {
  String ret;
  const int32_t size = static_cast<int32_t>(_columns.size());

  for (int32_t i = 0; i < size; i++) {
    if (i > 0) ret += L", ";

    ret += db_model::Helpers::Format(L"[%s]", _columns[i]->_name.c_str());
  }

  return ret;
}

bool Index::DependsOn(const String& column_name) {
  const auto find_it = std::ranges::find_if(_columns,
                                            [&](const ColumnRef& column) { return column->_name == column_name; });

  return find_it != _columns.end();
}

ColumnRef Table::FindColumn(const String& column_name) {
  const auto find_it = std::ranges::find_if(_columns,
                                            [&](const ColumnRef& column) { return column->_name == column_name; });

  if (find_it != _columns.end()) return *find_it;

  return nullptr;
}

String Procedure::GenerateCreateQuery() {
  const WCHAR* query = L"CREATE PROCEDURE [dbo].[%s] %s AS BEGIN %s END";
  const String param_string = GenerateParamString();

  return db_model::Helpers::Format(query, _name.c_str(), param_string.c_str(), _body.c_str());
}

String Procedure::GenerateAlterQuery() {
  const wchar_t* query = L"ALTER PROCEDURE [dbo].[%s] %s AS	BEGIN %s END";
  const String param_string = GenerateParamString();

  return db_model::Helpers::Format(query, _name.c_str(), param_string.c_str(), _body.c_str());
}

String Procedure::GenerateParamString() {
  String str;
  const int32_t size = static_cast<int32_t>(_parameters.size());

  for (int32_t i = 0; i < size; i++) {
    if (i < size - 1) str += db_model::Helpers::Format(L"\t%s %s,\n", _parameters[i].name.c_str(), _parameters[i].type.c_str());
    else str += db_model::Helpers::Format(L"\t%s %s", _parameters[i].name.c_str(), _parameters[i].type.c_str());
  }

  return str;
}

String Helpers::Format(const wchar_t* format, ...) {
  wchar_t buf[4096];

  va_list ap;
  va_start(ap, format);
  vswprintf_s(buf, 4096, format, ap);
  va_end(ap);

  return {buf};
}

String Helpers::DataTypeToString(const DataType type) {
  switch (type) {
    case DataType::kTinyInt: return L"TinyInt";
    case DataType::kSmallInt: return L"SmallInt";
    case DataType::kInt: return L"Int";
    case DataType::kReal: return L"Real";
    case DataType::kDateTime: return L"DateTime";
    case DataType::kFloat: return L"Float";
    case DataType::kBit: return L"Bit";
    case DataType::kNumeric: return L"Numeric";
    case DataType::kBigInt: return L"BigInt";
    case DataType::kVarBinary: return L"VarBinary";
    case DataType::kVarchar: return L"Varchar";
    case DataType::kBinary: return L"Binary";
    case DataType::kNVarChar: return L"NVarChar";
    default: return L"None";
  }
}

String Helpers::RemoveWhiteSpace(const String& str) {
  String ret = str;

  std::erase_if(ret, [=](const wchar_t ch) { return ::isspace(ch); });

  return ret;
}

DataType Helpers::StringToDataType(const wchar_t* str, int32_t& max_len) {
  const std::wregex reg(L"([a-z]+)(\\((max|\\d+)\\))?");
  std::wcmatch      ret;

  if (!std::regex_match(str, ret, reg)) return DataType::kNone;

  if (ret[3].matched) max_len = ::_wcsicmp(ret[3].str().c_str(), L"max") == 0 ? -1 : _wtoi(ret[3].str().c_str());
  else max_len = 0;

  if (::_wcsicmp(ret[1].str().c_str(), L"TinyInt") == 0) return DataType::kTinyInt;
  if (::_wcsicmp(ret[1].str().c_str(), L"SmallInt") == 0) return DataType::kSmallInt;
  if (::_wcsicmp(ret[1].str().c_str(), L"Int") == 0) return DataType::kInt;
  if (::_wcsicmp(ret[1].str().c_str(), L"Real") == 0) return DataType::kReal;
  if (::_wcsicmp(ret[1].str().c_str(), L"DateTime") == 0) return DataType::kDateTime;
  if (::_wcsicmp(ret[1].str().c_str(), L"Float") == 0) return DataType::kFloat;
  if (::_wcsicmp(ret[1].str().c_str(), L"Bit") == 0) return DataType::kBit;
  if (::_wcsicmp(ret[1].str().c_str(), L"Numeric") == 0) return DataType::kNumeric;
  if (::_wcsicmp(ret[1].str().c_str(), L"BigInt") == 0) return DataType::kBigInt;
  if (::_wcsicmp(ret[1].str().c_str(), L"VarBinary") == 0) return DataType::kVarBinary;
  if (::_wcsicmp(ret[1].str().c_str(), L"Varchar") == 0) return DataType::kVarchar;
  if (::_wcsicmp(ret[1].str().c_str(), L"Binary") == 0) return DataType::kBinary;
  if (::_wcsicmp(ret[1].str().c_str(), L"NVarChar") == 0) return DataType::kNVarChar;

  return DataType::kNone;
}
