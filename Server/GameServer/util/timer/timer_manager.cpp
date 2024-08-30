#include "pch.h"
#include "timer_manager.h"

#include "timer.h"

TimerManager::TimerManager() {
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);
  _frequency = frequency.QuadPart;
}

bool TimerManager::AddTimer(const String& name) {
  const auto timer = GetTimer(name);

  if (timer.has_value()) {
    return false;
  }

  if (!_timers.emplace(name, std::make_shared<Timer>(_frequency)).second) {
    return false;
  }

  return true;
}

float TimerManager::GetDeltaTime(const String& name) {
  const auto timer = GetTimer(name);

  if (!timer.has_value()) {
    return 0.0f;
  }

  timer.value()->Tick();
  return timer.value()->GetDeltaTime();
}

std::optional<std::shared_ptr<Timer>> TimerManager::GetTimer(const String& name) {
  const auto it = _timers.find(name);

  if (it != _timers.end()) {
    return it->second;
  }

  return std::nullopt;
}
