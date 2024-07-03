#pragma once

#define USE_MULTIPLE_LOCKS(count) Lock _locks[count];
#define USE_LOCK USE_MULTIPLE_LOCKS(1)

#define READ_LOCK_INDEX(index) ReadLockGuard _read_lock_##index(_locks[index], typeid(this).name());
#define READ_LOCK READ_LOCK_INDEX(0)

#define WRITE_LOCK_INDEX(index) WriteLockGuard _write_lock_##index(_locks[index], typeid(this).name());
#define WRITE_LOCK WRITE_LOCK_INDEX(0)

#define CRASH(cause) {                 \
  uint32_t* crash = nullptr;             \
  __analysis_assume(crash != nullptr); \
  *crash = 0xDEADC0DE;                 \
}

#define ASSERT_CRASH(expr) {            \
  if (!(expr)) {                        \
      CRASH("ASSERT_CRASH(" #expr ")"); \
      __analysis_assume(expr);          \
  }                                     \
}
