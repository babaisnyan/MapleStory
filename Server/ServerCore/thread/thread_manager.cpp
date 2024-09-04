#include "pch.h"
#include "thread_manager.h"

#include "core_tls.h"

#include "job/global_queue.h"
#include "job/job_queue.h"

ThreadManager::ThreadManager() {
  InitTls();
}

ThreadManager::~ThreadManager() {
  Join();
}

void ThreadManager::Launch(const std::function<void()>& callback) {
  LockGuard lock(_lock);

  _threads.emplace_back([=] {
    InitTls();
    callback();
    DestroyTls();
  });
}

void ThreadManager::Join() {
  for (std::thread& thread : _threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }

  _threads.clear();
}

void ThreadManager::InitTls() {
  static Atomic<uint32_t> next_thread_id = 1;
  LThreadId = next_thread_id.fetch_add(1);
}

void ThreadManager::DestroyTls() {}

void ThreadManager::FlushGlobalQueue() {
  while (true) {
    const uint64_t now = GetTickCount64();

    if (now > LEndTickCount) {
      break;
    }

    std::optional<JobQueueRef> queue = GlobalQueue::GetInstance().Pop();

    if (!queue.has_value()) {
      break;
    }

    queue.value()->Execute();
  }
}

void ThreadManager::DistributeReservedJobs() {
  const uint64_t now = GetTickCount64();
  JobTimer::GetInstance().Distribute(now);
}
