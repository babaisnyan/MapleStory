#include "pch.h"
#include "job_queue.h"

#include "global_queue.h"

void JobQueue::Push(JobRef job, const bool push_only) {
  const int32_t prev_count = _job_count.fetch_add(1);
  _job_queue.Push(std::move(job));

  if (prev_count == 0) {
    if (LJobQueue == nullptr && push_only) {
      Execute();
    } else {
      GlobalQueue::GetInstance().Push(shared_from_this());
    }
  }
}

void JobQueue::Execute() {
  LJobQueue = this;

  while (true) {
    Vector<JobRef> jobs;
    _job_queue.PopAll(jobs);

    const int32_t count = static_cast<int32_t>(jobs.size());

    for (int32_t i = 0; i < count; ++i) {
      jobs[i]->Execute();
    }

    if (_job_count.fetch_sub(count) == count) {
      LJobQueue = nullptr;
      return;
    }

    const uint64_t now = GetTickCount64();

    if (now >= LEndTickCount) {
      LJobQueue = nullptr;
      GlobalQueue::GetInstance().Push(shared_from_this());
      return;
    }
  }
}
