#pragma once

#include <cstdint>

namespace game {
  struct MsCoordinate {
    enum : uint8_t {
      kGridSize = 25
    };

    bool CheckTargetGridRange(const MsCoordinate& target, const int16_t range) const {
      return abs(grid_x - target.grid_x) <= range && abs(grid_y - target.grid_y) <= range;
    }

    bool CheckInRange(const MsCoordinate& target, const int32_t range_x, const int32_t range_y) const {
      return abs(x - target.x) <= (range_x / 2) && abs(y - target.y) <= (range_y / 2);
    }

    bool CheckInRangeDirection(const MsCoordinate& target, const int32_t range_x, const int32_t range_y, const bool check_right) const {
      if (check_right) {
        return x < target.x && abs(x - target.x) <= range_x && abs(y - target.y) <= range_y;
      }

      return x > target.x && abs(x - target.x) <= range_x && abs(y - target.y) <= range_y;
    }

    int32_t x;
    int32_t y;
    int16_t grid_x;
    int16_t grid_y;
  };
}
