#include "pch.h"
#include "dead_lock_profiler.h"

void DeadLockProfiler::PushLock(const char* name) {
  LockGuard guard(_lock);

  int32_t lock_id = 0;

  const auto find_it = _name_to_id.find(name);

  if (find_it == _name_to_id.end()) {
    lock_id = static_cast<int32_t>(_name_to_id.size());
    _name_to_id[name] = lock_id;
    _id_to_name[lock_id] = name;
  } else {
    lock_id = find_it->second;
  }

  if (!LLockStack.empty()) {
    const int32_t prev_id = LLockStack.top();

    if (lock_id != prev_id) {
      auto& history = _lock_history[prev_id];

      if (history.find(lock_id) == history.end()) {
        history.insert(lock_id);
        CheckCycle();
      }
    }
  }

  LLockStack.push(lock_id);
}

void DeadLockProfiler::PopLock(const char* name) {
  LockGuard guard(_lock);

  if (LLockStack.empty()) {
    CRASH("MULTIPLE_UNLOCK");
  }

  const int32_t lock_id = _name_to_id[name];

  if (LLockStack.top() != lock_id) {
    CRASH("INVALID_UNLOCK");
  }

  LLockStack.pop();
}

void DeadLockProfiler::CheckCycle() {
  const int32_t lock_count = static_cast<int32_t>(_name_to_id.size());
  _discovered_order = std::vector<int32_t>(lock_count, -1);
  _discovered_count = 0;
  _finished = std::vector<bool>(lock_count, false);
  _parent = std::vector<int32_t>(lock_count, -1);

  for (int32_t lock_id = 0; lock_id < lock_count; lock_id++) {
    Dfs(lock_id);
  }

  _discovered_order.clear();
  _finished.clear();
  _parent.clear();
}

void DeadLockProfiler::Dfs(const int32_t here) {
  if (_discovered_order[here] != -1) {
    return;
  }

  _discovered_order[here] = _discovered_count++;

  const auto find_it = _lock_history.find(here);

  if (find_it == _lock_history.end()) {
    _finished[here] = true;
    return;
  }

  const auto& next_set = find_it->second;

  for (const int32_t there : next_set) {
    if (_discovered_order[there] == -1) {
      _parent[there] = here;
      Dfs(there);
      continue;
    }

    if (_discovered_order[here] < _discovered_order[there]) {
      continue;
    }

    if (!_finished[there]) {
      printf("%s -> %s\n", _id_to_name[here], _id_to_name[there]);

      int32_t now = here;

      while (true) {
        printf("%s -> %s\n", _id_to_name[_parent[now]], _id_to_name[now]);
        now = _parent[now];

        if (now == there) {
          break;
        }
      }

      CRASH("DEADLOCK_DETECTED");
    }
  }

  _finished[here] = true;
}
