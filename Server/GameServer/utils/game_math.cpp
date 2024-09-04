#include "pch.h"
#include "game_math.h"

#include <cmath>
#include <mutex>
#include <numbers>

namespace {
  std::once_flag init_flag;
  std::array<float, 256> cos_table;
}

void Initialize() {
  for (int32_t i = 0; i < 256; ++i) {
    cos_table[i] = std::cos(i * std::numbers::pi_v<float> / 128.0f);
  }
}

float math::Cos256(const int32_t num) {
  std::call_once(init_flag, Initialize);
  return cos_table[num & 0xFF];
}

float math::Sin256(const int32_t num) {
  std::call_once(init_flag, Initialize);
  return cos_table[(num + 192) & 0xFF];
}
