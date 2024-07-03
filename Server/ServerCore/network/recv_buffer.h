#pragma once

class RecvBuffer {
  enum {
    kBufferCount = 10
  };

public:
  explicit RecvBuffer(int32_t buffer_size);
  ~RecvBuffer();

public:
  void Clear();
  bool OnRead(int32_t num_of_bytes);
  bool OnWrite(int32_t num_of_bytes);

  std::byte* GetReadPos() { return &_buffer[_read_pos]; }
  std::byte* GetWritePos() { return &_buffer[_write_pos]; }
  int32_t    GetDataSize() const { return _write_pos - _read_pos; }
  int32_t    GetFreeSize() const { return _capacity - _write_pos; }

private:
  int32_t           _capacity;
  int32_t           _buffer_size;
  int32_t           _read_pos = 0;
  int32_t           _write_pos = 0;
  Vector<std::byte> _buffer;
};
