#pragma once

class GlobalQueue {
public:
  static GlobalQueue& GetInstance() {
    static GlobalQueue instance;
    return instance;
  }

public:
  GlobalQueue();
  ~GlobalQueue();

  void                       Push(JobQueueRef job_queue);
  std::optional<JobQueueRef> Pop();

private:
  LockQueue<JobQueueRef> _queue;
};
