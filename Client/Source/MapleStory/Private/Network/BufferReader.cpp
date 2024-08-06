#include "Network/BufferReader.h"

BufferReader::BufferReader() {}

BufferReader::BufferReader(std::byte* Buffer, const uint32_t Size, const uint32_t Pos)
  : Buffer(Buffer), Size(Size), Pos(Pos) {}

BufferReader::~BufferReader() {}

bool BufferReader::Peek(void* Dest, const uint32_t Len) const {
  if (GetRemainSize() < Len) {
    return false;
  }

  memcpy(Dest, &Buffer[Pos], Len);

  return true;
}

bool BufferReader::Read(void* Dest, const uint32_t Len) {
  if (!Peek(Dest, Len)) {
    return false;
  }

  Pos += Len;

  return true;
}
