#pragma once

namespace game {
  class Timer : public std::enable_shared_from_this<Timer> {
  public:
    explicit Timer(uint64_t frequency);

  public:
    bool Start();
    void Tick();

    float GetDeltaTime() const;

  private:
    uint64_t _frequency;
    LARGE_INTEGER _start = {};
    LARGE_INTEGER _end = {};
    LARGE_INTEGER _check = {};

    float _delta_time = 0.0f;
  };
}
