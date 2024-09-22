#include "pch.h"
#include "string_utils.h"

#include <simdutf.h>

std::optional<std::string> utils::ConvertToUtf8(const String& wide_str) {
  if (wide_str.empty()) {
    return std::nullopt;
  }

  const auto size_needed = simdutf::utf8_length_from_utf16(reinterpret_cast<const char16_t*>(wide_str.c_str()), wide_str.length());

  if (size_needed <= 0) {
    return std::nullopt;
  }

  std::string utf8_str(size_needed, 0);
  const auto size = simdutf::convert_utf16_to_utf8(reinterpret_cast<const char16_t*>(wide_str.c_str()), wide_str.length(), utf8_str.data());

  if (size == 0) {
    return std::nullopt;
  }

  return utf8_str;
}

std::optional<String> utils::ConvertToWide(const std::string& utf8_str) {
  if (utf8_str.empty()) {
    return std::nullopt;
  }

  const auto size_needed = simdutf::utf16_length_from_utf8(utf8_str.c_str(), utf8_str.length());

  if (size_needed <= 0) {
    return std::nullopt;
  }

  std::wstring wide_str(size_needed, 0);
  const auto size = simdutf::convert_utf8_to_utf16(utf8_str.c_str(), utf8_str.length(), reinterpret_cast<char16_t*>(wide_str.data()));

  if (size == 0) {
    return std::nullopt;
  }

  return wide_str;
}
