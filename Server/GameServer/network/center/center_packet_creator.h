#pragma once

namespace game {
  class CenterPacketCreator {
  public:
    static SendBufferRef GetRegisterRequest(const std::string& name, const std::string& ip, uint16_t port);
    static SendBufferRef GetMigrationRequest(int32_t character_id, const std::string& player_ip, const std::string& server_name);
    static SendBufferRef GetMigrationSuccessResponse(int32_t character_id, int32_t auth_key);
    static SendBufferRef GetMigrationFailedResponse(int32_t character_id);
  };
}
