#include "pch.h"
#include "allocator.h"

#include "memory.h"

void* BaseAllocator::Alloc(const int32_t size) {
  return ::malloc(size);
}

void BaseAllocator::Release(void* ptr) {
  ::free(ptr);
}

void* StompAllocator::Alloc(const int32_t size) {
  const int64_t page_count = (size + kPageSize - 1) / kPageSize;
  const int64_t offset = page_count * kPageSize - size;
  void* base_address = ::VirtualAlloc(nullptr, page_count * kPageSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

  return static_cast<int8_t*>(base_address) + offset;
}

void StompAllocator::Release(void* ptr) {
  const int64_t address = reinterpret_cast<int64_t>(ptr);
  const int64_t base_address = address - (address % kPageSize);

  ::VirtualFree(reinterpret_cast<void*>(base_address), 0, MEM_RELEASE);
}

void* PoolAllocator::Alloc(const int32_t size) {
  return MemoryManager::GetInstance().Allocate(size);
}
void PoolAllocator::Release(void* ptr) {
  MemoryManager::GetInstance().Release(ptr);
}
