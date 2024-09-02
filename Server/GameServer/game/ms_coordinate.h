#pragma once
#include <cstdint>

struct MsCoordinate {
  enum : uint8_t {
    kGridSize = 25
  };

  float x;
  float y;
  int16_t grid_x;
  int16_t grid_y;
};
