#include "pch.h"
#include "xml_parser.h"

#include "memory/memory.h"
#include "utils/file_utils.h"
#include "utils/string_utils.h"

_locale_t kr = _create_locale(LC_NUMERIC, "kor");

bool XmlNode::GetBoolAttr(const wchar_t* key, const bool default_value) const {
  const XmlAttributeType* attr = _node->first_attribute(key);
  if (attr) return ::_wcsicmp(attr->value(), L"true") == 0;

  return default_value;
}

int8_t XmlNode::GetInt8Attr(const wchar_t* key, const int8_t default_value) const {
  const XmlAttributeType* attr = _node->first_attribute(key);
  if (attr) return static_cast<int8_t>(::_wtoi(attr->value()));

  return default_value;
}

int16_t XmlNode::GetInt16Attr(const wchar_t* key, const int16_t default_value) const {
  const XmlAttributeType* attr = _node->first_attribute(key);
  if (attr) return static_cast<int16_t>(::_wtoi(attr->value()));

  return default_value;
}

int32_t XmlNode::GetInt32Attr(const wchar_t* key, const int32_t default_value) const {
  const XmlAttributeType* attr = _node->first_attribute(key);
  if (attr) return ::_wtoi(attr->value());

  return default_value;
}

int64_t XmlNode::GetInt64Attr(const wchar_t* key, const int64_t default_value) const {
  const xml_attribute<wchar_t>* attr = _node->first_attribute(key);
  if (attr) return ::_wtoi64(attr->value());

  return default_value;
}

float XmlNode::GetFloatAttr(const wchar_t* key, const float default_value) const {
  const XmlAttributeType* attr = _node->first_attribute(key);
  if (attr) return static_cast<float>(::_wtof(attr->value()));

  return default_value;
}

double XmlNode::GetDoubleAttr(const wchar_t* key, const double default_value) const {
  const XmlAttributeType* attr = _node->first_attribute(key);
  if (attr) return ::_wtof_l(attr->value(), kr);

  return default_value;
}

const wchar_t* XmlNode::GetStringAttr(const wchar_t* key, const wchar_t* default_value) const {
  const XmlAttributeType* attr = _node->first_attribute(key);
  if (attr) return attr->value();

  return default_value;
}

bool XmlNode::GetBoolValue(const bool default_value) const {
  const wchar_t* val = _node->value();
  if (val) return ::_wcsicmp(val, L"true") == 0;

  return default_value;
}

int8_t XmlNode::GetInt8Value(const int8_t default_value) const {
  const wchar_t* val = _node->value();
  if (val) return static_cast<int8_t>(::_wtoi(val));

  return default_value;
}

int16_t XmlNode::GetInt16Value(const int16_t default_value) const {
  const wchar_t* val = _node->value();
  if (val) return static_cast<int16_t>(::_wtoi(val));
  return default_value;
}

int32_t XmlNode::GetInt32Value(const int32_t default_value) const {
  const wchar_t* val = _node->value();
  if (val) return ::_wtoi(val);

  return default_value;
}

int64_t XmlNode::GetInt64Value(const int64_t default_value) const {
  const wchar_t* val = _node->value();
  if (val) return ::_wtoi64(val);

  return default_value;
}

float XmlNode::GetFloatValue(const float default_value) const {
  const wchar_t* val = _node->value();
  if (val) return static_cast<float>(::_wtof(val));

  return default_value;
}

double XmlNode::GetDoubleValue(const double default_value) const {
  const wchar_t* val = _node->value();
  if (val) return ::_wtof_l(val, kr);

  return default_value;
}

const wchar_t* XmlNode::GetStringValue(const wchar_t* default_value) const {
  const wchar_t* val = _node->first_node()->value();
  if (val) return val;

  return default_value;
}

XmlNode XmlNode::FindChild(const wchar_t* key) const {
  return XmlNode(_node->first_node(key));
}

Vector<XmlNode> XmlNode::FindChildren(const wchar_t* key) const {
  Vector<XmlNode> nodes;

  xml_node<wchar_t>* node = _node->first_node(key);
  while (node) {
    nodes.emplace_back(node);
    node = node->next_sibling(key);
  }

  return nodes;
}


bool XmlParser::ParseFromFile(const wchar_t* path, XmlNode& root) {
  const Vector<std::byte>     bytes = FileUtils::ReadFile(path);
  const auto*                 utf8_data = reinterpret_cast<const char*>(bytes.data());
  const std::optional<String> wide_data = ConvertToWide(std::string(utf8_data));

  if (!wide_data.has_value()) return false;

  _data = wide_data.value();
  _document = MakeShared<XmlDocumentType>();
  _document->parse<0>(_data.data());
  root = XmlNode(_document->first_node());

  return true;
}
