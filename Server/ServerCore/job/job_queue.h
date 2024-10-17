#pragma once

#include "job.h"
#include "job_timer.h"

#include "utils/lock_queue.h"

class JobQueue : public std::enable_shared_from_this<JobQueue> {
public:
  void DoAsync(CallBackType&& callback) {
    Push(ObjectPool<Job>::MakeShared(std::move(callback)));
  }

  template <typename T, typename Ret, typename... Args>
  void DoAsync(Ret (T::*func)(Args...), Args&&... args) {
    std::shared_ptr<T> shared_obj = std::static_pointer_cast<T>(shared_from_this());
    JobQueue::Push(ObjectPool<Job>::MakeShared(shared_obj, func, std::forward<Args>(args)...));
  }

  void DoTimer(const uint64_t tick_after, CallBackType&& callback) {
    const auto job = ObjectPool<Job>::MakeShared(std::move(callback));
    JobTimer::GetInstance().Reserve(tick_after, shared_from_this(), job);
  }

  template <typename T, typename Ret, typename... Args>
  void DoTimer(const uint64_t tick_after, Ret (T::*func)(Args...), Args&&... args) {
    std::shared_ptr<T> shared_obj = std::static_pointer_cast<T>(shared_from_this());
    const JobRef job = ObjectPool<Job>::MakeShared(shared_obj, func, std::forward<Args>(args)...);

    JobTimer::GetInstance().Reserve(tick_after, shared_from_this(), job);
  }

  void Clear() {
    _job_queue.Clear();
  }

public:
  void Push(JobRef job, bool push_only = false);
  void Execute();

protected:
  LockQueue<JobRef> _job_queue;
  Atomic<int32_t> _job_count = 0;
};
