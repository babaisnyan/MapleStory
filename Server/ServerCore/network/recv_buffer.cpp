#include "pch.h"
#include "recv_buffer.h"

RecvBuffer::RecvBuffer(const int32_t buffer_size) : _buffer_size(buffer_size) {
  _capacity = buffer_size * kBufferCount;
  _buffer.resize(_capacity);
}

RecvBuffer::~RecvBuffer() {}

void RecvBuffer::Clear() {
  const int32_t data_size = GetDataSize();

  if (data_size == 0) {
    _read_pos = 0;
    _write_pos = 0;
  } else if (_buffer_size > GetFreeSize()) {
    memcpy(_buffer.data(), &_buffer[_read_pos], data_size);
    _read_pos = 0;
    _write_pos = data_size;
  }
}

bool RecvBuffer::OnRead(const int32_t num_of_bytes) {
  if (num_of_bytes > GetDataSize()) {
    return false;
  }

  _read_pos += num_of_bytes;
  return true;
}

bool RecvBuffer::OnWrite(const int32_t num_of_bytes) {
  if (num_of_bytes > GetFreeSize()) {
    return false;
  }

  _write_pos += num_of_bytes;
  return true;
}
