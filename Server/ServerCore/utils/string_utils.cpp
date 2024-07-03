#include "pch.h"
#include "string_utils.h"

std::optional<std::string> ConvertToUtf8(const String& wide_str) {
  if (wide_str.empty()) {
    return std::nullopt;
  }

  const int32_t size_needed = WideCharToMultiByte(CP_UTF8, 0, wide_str.c_str(), static_cast<int>(wide_str.size()), nullptr, 0, nullptr, nullptr);
  if (size_needed <= 0) {
    return std::nullopt;
  }

  std::string utf8_str(size_needed, 0);

  if (WideCharToMultiByte(CP_UTF8, 0, wide_str.c_str(), static_cast<int>(wide_str.size()), &utf8_str[0], size_needed, nullptr, nullptr) <= 0) {
    return std::nullopt;
  }

  return utf8_str;
}

std::optional<String> ConvertToWide(const std::string& utf8_str) {
  if (utf8_str.empty()) {
    return std::nullopt;
  }

  const int32_t size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), static_cast<int>(utf8_str.size()), nullptr, 0);
  if (size_needed <= 0) {
    return std::nullopt;
  }

  String wide_str(size_needed, 0);

  if (MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), static_cast<int>(utf8_str.size()), &wide_str[0], size_needed) <= 0) {
    return std::nullopt;
  }

  return wide_str;
}
