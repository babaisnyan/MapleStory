#pragma once

class BufferReader {
public:
  BufferReader();
  BufferReader(std::byte* buffer, uint32_t size, uint32_t pos = 0);
  ~BufferReader();

public:
  std::byte* GetBuffer() const noexcept { return _buffer; }
  uint32_t   GetSize() const noexcept { return _size; }
  uint32_t   GetReadSize() const noexcept { return _pos; }
  uint32_t   GetRemainSize() const noexcept { return _size - _pos; }

public:
  template <typename T>
  bool Peek(T* dest) { return Peek(dest, sizeof(T)); }

  bool Peek(void* dest, uint32_t len);

  template <typename T>
  bool Read(T* dest) { return Read(dest, sizeof(T)); }

  bool Read(void* dest, uint32_t len);

  template <typename T>
  BufferReader& operator>>(T& dest);

private:
  std::byte* _buffer = nullptr;
  uint32_t   _size = 0;
  uint32_t   _pos = 0;
};

template <typename T>
BufferReader& BufferReader::operator>>(T& dest) {
  dest = *reinterpret_cast<T*>(&_buffer[_pos]);
  _pos += sizeof(T);
  return *this;
}
