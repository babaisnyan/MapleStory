#include "pch.h"
#include "memory.h"
#include "memory_pool.h"

MemoryManager::MemoryManager() {
  int32_t size = 0;
  int32_t index = 0;

  for (size = 32; size <= 1024; size += 32) {
    MemoryPool* pool = new MemoryPool(size);
    _pools.emplace_back(pool);

    while (index <= size) {
      _pool_table[index++] = pool;
    }
  }

  for (size = 1024; size <= 2048; size += 128) {
    MemoryPool* pool = new MemoryPool(size);
    _pools.emplace_back(pool);

    while (index <= size) {
      _pool_table[index++] = pool;
    }
  }

  for (size = 2048; size <= 4096; size += 256) {
    MemoryPool* pool = new MemoryPool(size);
    _pools.emplace_back(pool);

    while (index <= size) {
      _pool_table[index++] = pool;
    }
  }
}

MemoryManager::~MemoryManager() {
  for (const MemoryPool* pool : _pools) {
    delete pool;
  }

  _pools.clear();
}

void* MemoryManager::Allocate(const int32_t size) const {
  MemoryHeader* header = nullptr;
  const int32_t alloc_size = size + sizeof(MemoryHeader);

  #ifdef _STOMP
  header = static_cast<MemoryHeader*>(StompAllocator::Alloc(alloc_size));
  #else
  if (alloc_size > kMaxAllocSize) {
    header = static_cast<MemoryHeader*>(_aligned_malloc(alloc_size, 16));
  } else {
    header = _pool_table[alloc_size]->Pop();
  }
  #endif

  return MemoryHeader::AddHeader(header, alloc_size);
}

void MemoryManager::Release(void* ptr) const {
  MemoryHeader* header = MemoryHeader::RemoveHeader(ptr);
  const int32_t alloc_size = header->alloc_size;
  ASSERT_CRASH(alloc_size > 0);

  #ifdef _STOMP
  StompAllocator::Release(header);
  #else
  if (alloc_size > kMaxAllocSize) {
    ::_aligned_free(header);
  } else {
    _pool_table[alloc_size]->Push(header);
  }
  #endif
}
