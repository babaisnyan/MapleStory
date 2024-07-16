#pragma once


class CenterPacketCreator {
public:
  static SendBufferRef GetRegisterRequest(const std::string& name, const std::string& ip, uint16_t port);
};
