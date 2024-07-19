#pragma once

struct ServerInfo;

class LoginPacketCreator {
public:
  static SendBufferRef GetSelectCharSuccessResponse(const std::string& ip, uint16_t port);
  static SendBufferRef GetSelectCharFailedResponse();
  
};
