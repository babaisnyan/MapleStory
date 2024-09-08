﻿#pragma once

namespace game {
  class SpawnPoint {
  public:
    SpawnPoint(const uint32_t id, const int32_t x, const int32_t y, const int32_t min_x, const int32_t max_x) : _id(id), _x(x), _y(y), _min_x(min_x), _max_x(max_x) {}
    SpawnPoint(const SpawnPoint& other) : _id(other._id), _x(other._x), _y(other._y), _min_x(other._min_x), _max_x(other._max_x) {}

    uint32_t GetId() const {
      return _id;
    }

    int32_t GetX() const {
      return _x;
    }

    int32_t GetY() const {
      return _y;
    }

    int32_t GetMinX() const {
      return _min_x;
    }

    int32_t GetMaxX() const {
      return _max_x;
    }

  private:
    uint32_t _id;
    int32_t _x;
    int32_t _y;
    int32_t _min_x;
    int32_t _max_x;
  };
}
