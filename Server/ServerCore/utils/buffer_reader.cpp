#include "pch.h"
#include "buffer_reader.h"

BufferReader::BufferReader() {}

BufferReader::BufferReader(std::byte* buffer, const uint32_t size, const uint32_t pos)
  : _buffer(buffer), _size(size), _pos(pos) {}

BufferReader::~BufferReader() {}

bool BufferReader::Peek(void* dest, const uint32_t len) {
  if (GetRemainSize() < len) {
    return false;
  }

  memcpy(dest, &_buffer[_pos], len);

  return true;
}

bool BufferReader::Read(void* dest, const uint32_t len) {
  if (!Peek(dest, len)) {
    return false;
  }

  _pos += len;

  return true;
}
