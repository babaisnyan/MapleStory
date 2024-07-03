#pragma once

class BaseAllocator {
public:
  static void* Alloc(int32_t size);
  static void Release(void* ptr);
};

class StompAllocator {
  enum {
    kPageSize = 0x1000
  };

public:
  static void* Alloc(int32_t size);
  static void Release(void* ptr);
};

class PoolAllocator {
public:
  static void* Alloc(int32_t size);
  static void Release(void* ptr);
};

template <typename T>
class StlAllocator {
public:
  using value_type = T;

  StlAllocator() {}

  template <typename Other>
  explicit StlAllocator(const StlAllocator<Other>&) {}

  T* allocate(size_t count) {
    const int32_t size = static_cast<int32_t>(count * sizeof(T));
    return static_cast<T*>(PoolAllocator::Alloc(size));
  }

  void deallocate(T* ptr, size_t count) {
    PoolAllocator::Release(ptr);
  }
};
