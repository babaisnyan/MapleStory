#include "pch.h"
#include "timer_manager.h"

TimerManager::TimerManager() {
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);
  _frequency = frequency.QuadPart;
}

bool TimerManager::AddTimer(const String& name) {
  return true;
}
