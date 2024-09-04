#pragma once

#include "types.h"

class Lock {
  enum :uint32_t {
    kAcquireTimeoutTick = 10000,
    kMaxSpinCount       = 5000,
    kWriteThreadMask    = 0xFFFF0000,
    kReadCountMast      = 0x0000FFFF,
    kEmptyFlag          = 0x00000000,
  };

public:
  void WriteLock(const char* name);
  void WriteUnlock(const char* name);

  void ReadLock(const char* name);
  void ReadUnlock(const char* name);

private:
  Atomic<uint32_t> _lock_flag = kEmptyFlag;
  uint16_t         _write_count = 0;
};

class ReadLockGuard {
public:
  explicit ReadLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) {
    _lock.ReadLock(_name);
  }

  ~ReadLockGuard() {
    _lock.ReadUnlock(_name);
  }

private:
  Lock&       _lock;
  const char* _name;
};

class WriteLockGuard {
public:
  explicit WriteLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) {
    _lock.WriteLock(_name);
  }

  ~WriteLockGuard() {
    _lock.WriteUnlock(_name);
  }

private:
  Lock&       _lock;
  const char* _name;
};
