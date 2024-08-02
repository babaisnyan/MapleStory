#pragma once
#include <cstdint>

struct MsCoordinate {
  int32_t x;
  int32_t y;
  int16_t grid_x;
  int16_t grid_y;
  int16_t grid_size = 1;
};
