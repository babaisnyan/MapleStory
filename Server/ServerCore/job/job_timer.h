#pragma once

struct JobData {
  JobData(std::weak_ptr<JobQueue> owner, JobRef job) :
    owner(std::move(owner)), job(std::move(job)) {}

  std::weak_ptr<JobQueue> owner;
  JobRef                  job;
};

struct TimerItem {
  bool operator<(const TimerItem& rhs) const {
    return execute_tick > rhs.execute_tick;
  }

  uint64_t execute_tick = 0;
  JobData* job_data = nullptr;
};

class JobTimer {
public:
  static JobTimer& GetInstance() {
    static JobTimer instance;
    return instance;
  }

public:
  void Reserve(uint64_t tick_after, std::weak_ptr<JobQueue> owner, JobRef job);
  void Distribute(uint64_t now);
  void Clear();

private:
  USE_LOCK;
  PriorityQueue<TimerItem> _items;
  Atomic<bool>             _distributing = false;
};
