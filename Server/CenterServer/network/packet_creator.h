#pragma once

namespace center {
  class PacketCreator {
  public:
    static SendBufferRef GetMigrationRequest(int32_t character_id);
    static SendBufferRef GetMigrationSuccessResponse(int32_t character_id, int32_t auth_key, const std::string& name, const std::string& ip, uint16_t port);
    static SendBufferRef GetMigrationFailedResponse(int32_t character_id);
  };
}
