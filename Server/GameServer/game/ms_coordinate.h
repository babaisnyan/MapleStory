#pragma once
#include <cstdint>

struct MsCoordinate {
  float x;
  float y;
  int16_t grid_x;
  int16_t grid_y;
  int16_t grid_size = 10;
};
