#include "pch.h"
#include "send_buffer.h"

#include "memory/memory.h"


SendBuffer::SendBuffer(SendBufferChunkRef owner, std::byte* buffer, const uint32_t alloc_size) :
  _buffer(buffer), _alloc_size(alloc_size), _owner(std::move(owner)) {}

void SendBuffer::Close(const uint32_t write_size) {
  ASSERT_CRASH(_alloc_size >= write_size);

  _write_size = write_size;
  _owner->Close(write_size);
}

void SendBufferChunk::Reset() {
  _open = false;
  _used_size = 0;
}

SendBufferRef SendBufferChunk::Open(const uint32_t alloc_size) {
  ASSERT_CRASH(alloc_size <= kChunkSize);
  ASSERT_CRASH(!_open);

  if (alloc_size > GetFreeSize()) {
    return nullptr;
  }

  _open = true;

  return ObjectPool<SendBuffer>::MakeShared(shared_from_this(), GetBuffer(), alloc_size);
}

void SendBufferChunk::Close(const uint32_t write_size) {
  ASSERT_CRASH(_open);
  _open = false;
  _used_size += write_size;
}

SendBufferRef SendBufferManager::Open(const uint32_t size) {
  if (!LSendBufferChunk) {
    LSendBufferChunk = Pop();
    LSendBufferChunk->Reset();
  }

  ASSERT_CRASH(!LSendBufferChunk->IsOpen())

  if (LSendBufferChunk->GetFreeSize() < size) {
    LSendBufferChunk = Pop();
    LSendBufferChunk->Reset();
  }

  return LSendBufferChunk->Open(size);
}

SendBufferChunkRef SendBufferManager::Pop() {
  {
    WRITE_LOCK;

    if (!_send_buffer_chunks.empty()) {
      SendBufferChunkRef chunk = _send_buffer_chunks.back();
      _send_buffer_chunks.pop_back();
      return chunk;
    }
  }

  return {MsNew<SendBufferChunk>(), PushGlobal};
}

void SendBufferManager::Push(const SendBufferChunkRef& buffer) {
  WRITE_LOCK;
  _send_buffer_chunks.push_back(buffer);
}

void SendBufferManager::PushGlobal(SendBufferChunk* buffer) {
  std::cout << "PushGlobal\n";

  GetInstance().Push({buffer, PushGlobal});
}
