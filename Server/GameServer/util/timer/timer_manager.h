#pragma once

namespace game {
  class Timer;

  class TimerManager {
  private:
    TimerManager();
    ~TimerManager() = default;

  public:
    static TimerManager& GetInstance() {
      static TimerManager instance;
      return instance;
    }

  public:
    bool AddTimer(const String& name);
    float GetDeltaTime(const String& name);

  private:
    std::optional<std::shared_ptr<Timer>> GetTimer(const String& name);

  private:
    std::unordered_map<String, std::shared_ptr<Timer>> _timers;
    uint64_t _frequency;
  };
}
