#pragma once

#include "memory_pool.h"
#include "types.h"

template <typename T>
class ObjectPool {
public:
  template <typename... Args>
  static T* Pop(Args&&... args) {
    #ifdef _STOMP
    MemoryHeader* ptr = static_cast<MemoryHeader*>(StompAllocator::Alloc(_alloc_size));
    T* memory = static_cast<T*>(MemoryHeader::AddHeader(ptr, _alloc_size));
    #else
    T* memory = static_cast<T*>(MemoryHeader::AddHeader(_pool.Pop(), _alloc_size));
    #endif

    new(memory)T(std::forward<Args>(args)...);
    return memory;
  }

  static void Push(T* obj) {
    obj->~T();

    #ifdef _STOMP
    StompAllocator::Release(MemoryHeader::RemoveHeader(obj));
    #else
    _pool.Push(MemoryHeader::RemoveHeader(obj));
    #endif
  }

  template <typename... Args>
  static std::shared_ptr<T> MakeShared(Args&&... args) {
    std::shared_ptr<T> ptr = {Pop(std::forward<Args>(args)...), Push};
    return ptr;
  }

private:
  static int32_t _alloc_size;
  static MemoryPool _pool;
};

template <typename T>
int32_t ObjectPool<T>::_alloc_size = sizeof(T) + sizeof(MemoryHeader);

template <typename T>
MemoryPool ObjectPool<T>::_pool{_alloc_size};
