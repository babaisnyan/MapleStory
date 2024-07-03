#pragma once
#include "types.h"
#include "memory/container.h"

class SendBufferChunk;

class SendBuffer : public std::enable_shared_from_this<SendBuffer> {
public:
  explicit SendBuffer(SendBufferChunkRef owner, std::byte* buffer, uint32_t alloc_size);
  ~SendBuffer() = default;

public:
  std::byte* GetBuffer() const noexcept { return _buffer; }
  uint32_t   GetWriteSize() const noexcept { return _write_size; }
  uint32_t   GetAllocSize() const noexcept { return _alloc_size; }
  void       Close(uint32_t write_size);

private:
  std::byte*         _buffer;
  uint32_t           _alloc_size = 0;
  uint32_t           _write_size = 0;
  SendBufferChunkRef _owner;
};

class SendBufferChunk : public std::enable_shared_from_this<SendBufferChunk> {
  enum {
    kChunkSize = 1024 * 4
  };

public:
  SendBufferChunk() = default;
  ~SendBufferChunk() = default;

  void          Reset();
  SendBufferRef Open(uint32_t alloc_size);
  void          Close(uint32_t write_size);

  bool IsOpen() const noexcept { return _open; }

  std::byte* GetBuffer() noexcept { return &_buffer[_used_size]; }

  uint32_t GetFreeSize() const noexcept { return static_cast<uint32_t>(_buffer.size() - _used_size); }

private:
  Array<std::byte, kChunkSize> _buffer = {};
  bool                         _open = false;
  uint32_t                     _used_size = 0;
};

class SendBufferManager {
public:
  static SendBufferManager& GetInstance() {
    static SendBufferManager instance;
    return instance;
  }

public:
  SendBufferRef Open(uint32_t size);

private:
  SendBufferChunkRef Pop();
  void               Push(const SendBufferChunkRef& buffer);

  static void PushGlobal(SendBufferChunk* buffer);

private:
  USE_LOCK;
  Vector<SendBufferChunkRef> _send_buffer_chunks;
};
