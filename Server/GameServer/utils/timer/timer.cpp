#include "pch.h"
#include "timer.h"

Timer::Timer(const uint64_t frequency) : _frequency(frequency) {}

bool Timer::Start() {
  if (_frequency == 0) {
    return false;
  }

  QueryPerformanceCounter(&_start);
  QueryPerformanceCounter(&_end);
  QueryPerformanceCounter(&_check);

  return true;
}

void Timer::Tick() {
  QueryPerformanceCounter(&_end);

  if (_end.QuadPart - _check.QuadPart > _frequency) {
    _check = _end;
  }

  _delta_time = static_cast<float>(_end.QuadPart - _start.QuadPart) / _frequency;
  _start = _end;
}

float Timer::GetDeltaTime() const {
  return _delta_time;
}
