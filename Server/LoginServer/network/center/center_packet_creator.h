#pragma once


class CenterPacketCreator {
public:
  static SendBufferRef GetRegisterRequest(const std::string& name, const std::string& ip, uint16_t port);
  static SendBufferRef GetMigrationRequest(int32_t character_id, const std::string& player_ip, const std::string& server_name);
};
