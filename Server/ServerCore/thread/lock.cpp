#include "pch.h"

#include "lock.h"
#include "core_tls.h"
#include "dead_lock_profiler.h"

void Lock::WriteLock(const char* name) {
  #if _DEBUG
  DeadLockProfiler::GetInstance().PushLock(name);
  #endif

  const uint32_t lock_thread_id = (_lock_flag.load() & kWriteThreadMask) >> 16;

  if (lock_thread_id == LThreadId) {
    _write_count++;
    return;
  }

  const uint64_t begin_tick = ::GetTickCount64();
  const uint32_t desired = LThreadId << 16 & kWriteThreadMask;

  while (true) {
    for (uint32_t spin_count = 0; spin_count < kMaxSpinCount; spin_count++) {
      uint32_t expected = kEmptyFlag;

      if (_lock_flag.compare_exchange_strong(expected, desired)) {
        _write_count++;
        return;
      }
    }

    if (::GetTickCount64() - begin_tick >= kAcquireTimeoutTick) {
      CRASH("LOCK_TIMEOUT");
    }

    std::this_thread::yield();
  }
}

void Lock::WriteUnlock(const char* name) {
  #if _DEBUG
  DeadLockProfiler::GetInstance().PopLock(name);
  #endif

  if ((_lock_flag.load() & kReadCountMast) != 0) {
    CRASH("INVALID_UNLOCK_ORDER");
  }

  const uint16_t lock_count = --_write_count;

  if (lock_count == 0) {
    _lock_flag.store(kEmptyFlag);
  }
}

void Lock::ReadLock(const char* name) {
  #if _DEBUG
  DeadLockProfiler::GetInstance().PushLock(name);
  #endif

  const uint32_t lock_thread_id = (_lock_flag.load() & kWriteThreadMask) >> 16;

  if (lock_thread_id == LThreadId) {
    _lock_flag.fetch_add(1);
    return;
  }

  const uint64_t begin_tick = ::GetTickCount64();

  while (true) {
    for (uint32_t spin_count = 0; spin_count < kMaxSpinCount; spin_count++) {
      uint32_t expected = _lock_flag.load() & kReadCountMast;

      if (_lock_flag.compare_exchange_strong(expected, expected + 1)) {
        return;
      }
    }

    if (::GetTickCount64() - begin_tick >= kAcquireTimeoutTick) {
      CRASH("LOCK_TIMEOUT");
    }

    std::this_thread::yield();
  }
}

void Lock::ReadUnlock(const char* name) {
  #if _DEBUG
  DeadLockProfiler::GetInstance().PopLock(name);
  #endif

  if ((_lock_flag.fetch_sub(1) & kReadCountMast) == 0) {
    CRASH("MULTIPLE_UNLOCK");
  }
}
