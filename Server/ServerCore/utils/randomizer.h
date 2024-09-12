#pragma once

namespace utils::random {
  int64_t Rand(int64_t max);
  int64_t Rand(int64_t start, int64_t end);
  float RandFloat(float start, float end);

  bool IsSuccess(int32_t chance);
}
