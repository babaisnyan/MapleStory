#pragma once

class BufferWriter {
public:
  BufferWriter();
  BufferWriter(std::byte* buffer, uint32_t size, uint32_t pos = 0);
  ~BufferWriter();

public:
  std::byte* GetBuffer() const noexcept { return _buffer; }
  uint32_t   GetSize() const noexcept { return _size; }
  uint32_t   GetWriteSize() const noexcept { return _pos; }
  uint32_t   GetFreeSize() const noexcept { return _size - _pos; }

public:
  template <typename T>
  bool Write(T* src) { return Write(src, sizeof(T)); }

  bool Write(const void* src, uint32_t len);

  template <typename T>
  T* Reserve(uint16_t count = 1);

  template <typename T>
  BufferWriter& operator<<(T&& src);

private:
  std::byte* _buffer = nullptr;
  uint32_t   _size = 0;
  uint32_t   _pos = 0;
};

template <typename T>
T* BufferWriter::Reserve(const uint16_t count) {
  if (GetFreeSize() < sizeof(T) * count) {
    return nullptr;
  }

  T* ret = reinterpret_cast<T*>(&_buffer[_pos]);
  _pos += sizeof(T) * count;

  return ret;
}

template <typename T>
BufferWriter& BufferWriter::operator<<(T&& src) {
  using Type = std::remove_reference_t<T>;

  *reinterpret_cast<Type*>(&_buffer[_pos]) = std::forward<Type>(src);
  _pos += sizeof(T);
  return *this;
}
