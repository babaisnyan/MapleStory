#include "pch.h"
#include "job_timer.h"

#include "job_queue.h"

void JobTimer::Reserve(const uint64_t tick_after, std::weak_ptr<JobQueue> owner, JobRef job) {
  const uint64_t execute_tick = GetTickCount64() + tick_after;
  auto job_data = ObjectPool<JobData>::Pop(owner, job);

  WRITE_LOCK;

  _items.push(TimerItem{execute_tick, job_data});
}

void JobTimer::Distribute(const uint64_t now) {
  if (_distributing.exchange(true)) {
    return;
  }

  Vector<TimerItem> items;
  {
    WRITE_LOCK;

    while (!_items.empty()) {
      const auto& item = _items.top();

      if (now < item.execute_tick) {
        break;
      }

      items.push_back(item);
      _items.pop();
    }
  }

  for (const auto& item : items) {
    if (const auto owner = item.job_data->owner.lock()) {
      owner->Push(item.job_data->job, true);
    }

    ObjectPool<JobData>::Push(item.job_data);
  }

  _distributing.store(false);
}

void JobTimer::Clear() {
  WRITE_LOCK;

  while (!_items.empty()) {
    const auto& item = _items.top();
    ObjectPool<JobData>::Push(item.job_data);
    _items.pop();
  }
}
