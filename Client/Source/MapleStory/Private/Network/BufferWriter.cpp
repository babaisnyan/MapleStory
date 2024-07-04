#include "Network/BufferWriter.h"

BufferWriter::BufferWriter() {}

BufferWriter::BufferWriter(std::byte* Buffer, const uint32_t Size, const uint32_t Pos)
  : Buffer(Buffer), Size(Size), Pos(Pos) {}

BufferWriter::~BufferWriter() {}

bool BufferWriter::Write(const void* Src, const uint32_t Len) {
  if (GetFreeSize() < Len) {
    return false;
  }
  
  memcpy(&Buffer[Pos], Src, Len);
  Pos += Len;

  return true;
}
