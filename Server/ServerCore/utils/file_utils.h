#pragma once
#include <vector>

class FileUtils {
public:
  static Vector<std::byte> ReadFile(const wchar_t* path);
};
