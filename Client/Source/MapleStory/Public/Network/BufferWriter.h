#pragma once

class BufferWriter {
public:
  BufferWriter();
  BufferWriter(std::byte* Buffer, uint32_t Size, uint32_t Pos = 0);
  ~BufferWriter();

public:
  std::byte* GetBuffer() const noexcept { return Buffer; }
  uint32_t   GetSize() const noexcept { return Size; }
  uint32_t   GetWriteSize() const noexcept { return Pos; }
  uint32_t   GetFreeSize() const noexcept { return Size - Pos; }

public:
  template <typename T>
  bool Write(T* Src) { return Write(Src, sizeof(T)); }

  bool Write(const void* Src, uint32_t Len);

  template <typename T>
  T* Reserve(uint16_t Count = 1);

  template <typename T>
  BufferWriter& operator<<(T&& Src);

private:
  std::byte* Buffer = nullptr;
  uint32_t   Size = 0;
  uint32_t   Pos = 0;
};

template <typename T>
T* BufferWriter::Reserve(const uint16_t Count) {
  if (GetFreeSize() < sizeof(T) * Count) {
    return nullptr;
  }

  T* Ret = reinterpret_cast<T*>(&Buffer[Pos]);
  Pos += sizeof(T) * Count;

  return Ret;
}

template <typename T>
BufferWriter& BufferWriter::operator<<(T&& Src) {
  using Type = std::remove_reference_t<T>;

  *reinterpret_cast<Type*>(&Buffer[Pos]) = std::forward<Type>(Src);
  Pos += sizeof(T);
  return *this;
}
