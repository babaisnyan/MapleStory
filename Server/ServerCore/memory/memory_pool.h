#pragma once

__declspec(align(16)) struct MemoryHeader : public SLIST_ENTRY {
  explicit MemoryHeader(const int32_t size) : alloc_size(size) {

  }

  static void* AddHeader(MemoryHeader* header, const int32_t size) {
    new(header) MemoryHeader(size);
    return ++header;
  }

  static MemoryHeader* RemoveHeader(void* ptr) {
    MemoryHeader* header = static_cast<MemoryHeader*>(ptr) - 1;
    return header;
  }

  int32_t alloc_size;
};

__declspec(align(16)) class MemoryPool {
public:
  explicit MemoryPool(int32_t size);
  ~MemoryPool();

public:
  void Push(MemoryHeader* ptr);
  MemoryHeader* Pop();

private:
  SLIST_HEADER _header;

  int32_t _alloc_size = 0;
  Atomic<int32_t> _use_count = 0;
  Atomic<int32_t> _reserve_count = 0;
};
