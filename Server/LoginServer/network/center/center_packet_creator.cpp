#include "pch.h"
#include "center_packet_creator.h"

#include "center_server_packet_handler.h"

#include "network/protocol/center_protocol.pb.h"

#include "utils/constants.h"

SendBufferRef CenterPacketCreator::GetRegisterRequest(const std::string& name, const std::string& ip, const uint16_t port) {
  protocol::CenterClientRegisterRequest packet;
  packet.set_version(constants::kVersion);
  packet.set_name(name);
  packet.set_ip(ip);
  packet.set_port(port);
  packet.set_type(protocol::SERVER_TYPE_LOGIN);

  const auto send_buffer = CenterServerPacketHandler::MakeSendBuffer(packet);
  return send_buffer;
}
