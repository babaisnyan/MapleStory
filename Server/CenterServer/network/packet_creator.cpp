#include "pch.h"
#include "packet_creator.h"

#include "center_client_packet_handler.h"

#include "protocol/center_protocol.pb.h"

using namespace protocol;

SendBufferRef PacketCreator::GetMigrationRequest(const int32_t character_id) {
  CenterServerMigrationRequest packet;
  packet.set_character_id(character_id);

  return CenterClientPacketHandler::MakeSendBuffer(packet);
}

SendBufferRef PacketCreator::GetMigrationSuccessResponse(const int32_t character_id, const int32_t auth_key, const std::string& name, const std::string& ip, const uint16_t port) {
  CenterServerMigrationResponse packet;
  packet.set_success(true);
  packet.set_character_id(character_id);
  packet.set_auth_key(auth_key);

  const auto server = packet.mutable_server();
  server->set_name(name);
  server->set_ip(ip);
  server->set_port(port);

  return CenterClientPacketHandler::MakeSendBuffer(packet);
}

SendBufferRef PacketCreator::GetMigrationFailedResponse(int32_t character_id) {
  CenterServerMigrationResponse packet;
  packet.set_success(false);
  packet.set_character_id(character_id);

  return CenterClientPacketHandler::MakeSendBuffer(packet);
}
