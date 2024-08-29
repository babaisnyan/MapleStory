#pragma once

namespace game {
  class SpawnPoint {
  public:
    SpawnPoint(const uint32_t id, const float x, const float y): _id(id), _x(x), _y(y) {}

    uint32_t GetId() const {
      return _id;
    }

    float GetX() const {
      return _x;
    }

    float GetY() const {
      return _y;
    }

  private:
    uint32_t _id;
    float _x;
    float _y;
  };
}
