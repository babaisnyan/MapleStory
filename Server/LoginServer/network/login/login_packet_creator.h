﻿#pragma once

namespace login {
  class LoginPacketCreator {
  public:
    static SendBufferRef GetSelectCharSuccessResponse(const std::string& ip, uint16_t port, int32_t auth_key, int32_t character_id);
    static SendBufferRef GetSelectCharFailedResponse();
  };
}
