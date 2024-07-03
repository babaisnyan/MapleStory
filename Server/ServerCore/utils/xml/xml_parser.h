#pragma once
#include "rapidxml.hpp"

using namespace rapidxml;

using XmlNodeType = xml_node<wchar_t>;
using XmlDocumentType = xml_document<wchar_t>;
using XmlAttributeType = xml_attribute<wchar_t>;

class XmlNode {
public:
  explicit XmlNode(XmlNodeType* node = nullptr) : _node(node) {}

  bool IsValid() const { return _node != nullptr; }

  bool           GetBoolAttr(const wchar_t* key, bool default_value = false) const;
  int8_t         GetInt8Attr(const wchar_t* key, int8_t default_value = 0) const;
  int16_t        GetInt16Attr(const wchar_t* key, int16_t default_value = 0) const;
  int32_t        GetInt32Attr(const wchar_t* key, int32_t default_value = 0) const;
  int64_t        GetInt64Attr(const wchar_t* key, int64_t default_value = 0) const;
  float          GetFloatAttr(const wchar_t* key, float default_value = 0.0f) const;
  double         GetDoubleAttr(const wchar_t* key, double default_value = 0.0) const;
  const wchar_t* GetStringAttr(const wchar_t* key, const wchar_t* default_value = L"") const;

  bool           GetBoolValue(bool default_value = false) const;
  int8_t         GetInt8Value(int8_t default_value = 0) const;
  int16_t        GetInt16Value(int16_t default_value = 0) const;
  int32_t        GetInt32Value(int32_t default_value = 0) const;
  int64_t        GetInt64Value(int64_t default_value = 0) const;
  float          GetFloatValue(float default_value = 0.0f) const;
  double         GetDoubleValue(double default_value = 0.0) const;
  const wchar_t* GetStringValue(const wchar_t* default_value = L"") const;

  XmlNode         FindChild(const wchar_t* key) const;
  Vector<XmlNode> FindChildren(const wchar_t* key) const;

private:
  XmlNodeType* _node = nullptr;
};

class XmlParser {
public:
  bool ParseFromFile(const wchar_t* path, XmlNode& root);

private:
  std::shared_ptr<XmlDocumentType> _document = nullptr;
  String                           _data;
};
