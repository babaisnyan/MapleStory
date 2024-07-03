#pragma once

#include "allocator.h"

class MemoryPool;

class MemoryManager {
  enum {
    kPoolCount    = (1024 / 32) + (1024 / 128) + (2048 / 256),
    kMaxAllocSize = 4096
  };

private:
  MemoryManager();
  ~MemoryManager();

public:
  static MemoryManager& GetInstance() {
    static MemoryManager instance;
    return instance;
  }

public:
  [[nodiscard]] void* Allocate(int32_t size) const;
  void                Release(void* ptr) const;

private:
  std::vector<MemoryPool*> _pools;
  MemoryPool*              _pool_table[kMaxAllocSize + 1];
};

template <typename Type, typename... Args>
Type* MsNew(Args&&... args) {
  Type* memory = static_cast<Type*>(PoolAllocator::Alloc(sizeof(Type)));
  new(memory)Type(std::forward<Args>(args)...);
  return memory;
}

template <typename Type>
void MsDelete(Type* ptr) {
  ptr->~Type();
  PoolAllocator::Release(ptr);
}

template <typename Type, typename... Args>
std::shared_ptr<Type> MakeShared(Args&&... args) {
  return std::shared_ptr<Type>{MsNew<Type>(std::forward<Args>(args)...), MsDelete<Type>};
}
