#include "pch.h"
#include "memory_pool.h"

MemoryPool::MemoryPool(const int32_t size) : _alloc_size(size) {
  InitializeSListHead(&_header);
}

MemoryPool::~MemoryPool() {
  while (MemoryHeader* memory = static_cast<MemoryHeader*>(InterlockedPopEntrySList(&_header))) {
    _aligned_free(memory);
  }
}

void MemoryPool::Push(MemoryHeader* ptr) {
  ptr->alloc_size = 0;
  InterlockedPushEntrySList(&_header, ptr);
  _use_count.fetch_sub(1);
  _reserve_count.fetch_add(1);
}

MemoryHeader* MemoryPool::Pop() {
  MemoryHeader* memory = static_cast<MemoryHeader*>(InterlockedPopEntrySList(&_header));

  if (memory == nullptr) {
    memory = static_cast<MemoryHeader*>(_aligned_malloc(_alloc_size, 16));
  } else {
    ASSERT_CRASH(memory->alloc_size == 0);
    _reserve_count.fetch_sub(1);
  }

  _use_count.fetch_add(1);

  return memory;
}
