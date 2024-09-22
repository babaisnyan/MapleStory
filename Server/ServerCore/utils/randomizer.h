#pragma once

namespace utils::random {
  int64_t Rand(int64_t max);
  int64_t Rand(int64_t start, int64_t end);
  double RandDouble(double start, double end);

  bool IsSuccess(int32_t chance);
}
