#pragma once

namespace utils {
  std::optional<std::string> ConvertToUtf8(const String& wide_str);
  std::optional<String> ConvertToWide(const std::string& utf8_str);
}
