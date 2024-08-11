#pragma once
#include <cstdint>

struct MsCoordinate {
  double x;
  double y;
  int16_t grid_x;
  int16_t grid_y;
  int16_t grid_size = 1;
};
