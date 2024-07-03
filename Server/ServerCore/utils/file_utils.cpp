#include "pch.h"
#include "file_utils.h"

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

Vector<std::byte> FileUtils::ReadFile(const wchar_t* path) {
  Vector<std::byte> ret;

  const fs::path file_path{path};

  const uint32_t file_size = static_cast<uint32_t>(fs::file_size(file_path));
  ret.resize(file_size);

  std::basic_ifstream<std::byte> input_stream{file_path};
  input_stream.read(ret.data(), file_size);

  return ret;
}