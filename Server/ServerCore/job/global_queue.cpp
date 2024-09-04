#include "pch.h"
#include "global_queue.h"

GlobalQueue::GlobalQueue() {}

GlobalQueue::~GlobalQueue() {}

void GlobalQueue::Push(JobQueueRef job_queue) {
  _queue.Push(std::move(job_queue));
}

std::optional<JobQueueRef> GlobalQueue::Pop() {
  return _queue.Pop();
}
