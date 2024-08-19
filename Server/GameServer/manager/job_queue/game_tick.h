#pragma once
#include <tbb/concurrent_queue.h>

namespace game {
  class MapInstance;

  class GameTick : public std::enable_shared_from_this<GameTick> {
  public:
    static std::shared_ptr<GameTick>& GetInstance() {
      static auto instance = std::make_shared<GameTick>();
      return instance;
    }

    void Start();

    template <typename T, typename Ret, typename... Args>
    void DoAsync(Ret (T::*func)(Args...), Args&&... args);

  private:
    std::atomic<bool> _is_running = true;
    tbb::concurrent_queue<JobRef> _job_queue;
  };

  template <typename T, typename Ret, typename... Args>
  void GameTick::DoAsync(Ret (T::*func)(Args...), Args&&... args) {
    const auto shared_obj = std::static_pointer_cast<T>(shared_from_this());
    _job_queue.push(ObjectPool<Job>::MakeShared(shared_obj, func, std::forward<Args>(args)...));
  }
}
