﻿#include "pch.h"
#include "randomizer.h"

int64_t utils::random::Rand(const int64_t max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());

  std::uniform_int_distribution<int64_t> dist(0, max);

  return dist(gen);
}

int64_t utils::random::Rand(const int64_t min, const int64_t max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());

  std::uniform_int_distribution dist(min, max);

  return dist(gen);
}

bool utils::random::IsSuccess(const int32_t chance) {
  return Rand(100) < chance;
}
