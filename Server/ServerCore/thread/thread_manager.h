#pragma once

#include <functional>
#include <thread>

class ThreadManager {
public:
  ThreadManager();
  ~ThreadManager();

  static ThreadManager& GetInstance() {
    static ThreadManager instance;
    return instance;
  }

  void Launch(const std::function<void()>& callback);
  void Join();

  static void InitTls();
  static void DestroyTls();

  static void FlushGlobalQueue();
  static void DistributeReservedJobs();

private:
  Mutex                    _lock;
  std::vector<std::thread> _threads;
};
