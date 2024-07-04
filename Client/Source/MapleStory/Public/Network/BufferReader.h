#pragma once

class BufferReader {
public:
  BufferReader();
  BufferReader(std::byte* Buffer, uint32_t Size, uint32_t Pos = 0);
  ~BufferReader();

public:
  std::byte* GetBuffer() const noexcept { return Buffer; }
  uint32_t   GetSize() const noexcept { return Size; }
  uint32_t   GetReadSize() const noexcept { return Pos; }
  uint32_t   GetRemainSize() const noexcept { return Size - Pos; }

public:
  template <typename T>
  bool Peek(T* Dest) { return Peek(Dest, sizeof(T)); }

  bool Peek(void* Dest, uint32_t Len);

  template <typename T>
  bool Read(T* Dest) { return Read(Dest, sizeof(T)); }

  bool Read(void* Dest, uint32_t Len);

  template <typename T>
  BufferReader& operator>>(T& Dest);

private:
  std::byte* Buffer = nullptr;
  uint32_t   Size = 0;
  uint32_t   Pos = 0;
};

template <typename T>
BufferReader& BufferReader::operator>>(T& Dest) {
  Dest = *reinterpret_cast<T*>(&Buffer[Pos]);
  Pos += sizeof(T);
  return *this;
}
