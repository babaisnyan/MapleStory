﻿#pragma once

namespace game::random {
  int64_t Rand(int64_t max);
  int64_t Rand(int64_t min, int64_t max);

  bool IsSuccess(int32_t chance);
}
