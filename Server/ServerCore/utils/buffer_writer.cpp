#include "pch.h"
#include "buffer_writer.h"

BufferWriter::BufferWriter() {}

BufferWriter::BufferWriter(std::byte* buffer, const uint32_t size, const uint32_t pos)
  : _buffer(buffer), _size(size), _pos(pos) {}

BufferWriter::~BufferWriter() {}

bool BufferWriter::Write(const void* src, const uint32_t len) {
  if (GetFreeSize() < len) {
    return false;
  }

  memcpy(&_buffer[_pos], src, len);
  _pos += len;

  return true;
}
