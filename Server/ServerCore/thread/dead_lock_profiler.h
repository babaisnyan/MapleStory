#pragma once

#include <map>
#include <vector>

class DeadLockProfiler {
private:
  DeadLockProfiler() = default;
  ~DeadLockProfiler() = default;

public:
  static DeadLockProfiler& GetInstance() {
    static DeadLockProfiler instance;
    return instance;
  }

public:
  void PushLock(const char* name);
  void PopLock(const char* name);
  void CheckCycle();

private:
  void Dfs(int32_t here);

private:
  std::unordered_map<const char*, int32_t> _name_to_id;
  std::unordered_map<int32_t, const char*> _id_to_name;
  std::map<int32_t, std::set<int32_t>>     _lock_history;

  Mutex _lock;

private:
  std::vector<int32_t> _discovered_order;
  int32_t              _discovered_count = 0;
  std::vector<bool>    _finished;
  std::vector<int32_t> _parent;
};
