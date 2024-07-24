#include "pch.h"
#include "login_packet_creator.h"

#include "login_client_packet_handler.h"

#include "network/protocol/login_protocol.pb.h"

using namespace protocol;

SendBufferRef LoginPacketCreator::GetSelectCharSuccessResponse(const std::string& ip, const uint16_t port) {
  LoginServerCharSelectResult packet;
  packet.set_result(SELECT_CHAR_RESULT_SUCCESS);
  // packet.set_ip(ip);
  // packet.set_port(port);

  const auto send_buffer = LoginClientPacketHandler::MakeSendBuffer(packet);
  return send_buffer;
}

SendBufferRef LoginPacketCreator::GetSelectCharFailedResponse() {
  LoginServerCharSelectResult packet;
  packet.set_result(SELECT_CHAR_RESULT_SERVER_ERROR);

  const auto send_buffer = LoginClientPacketHandler::MakeSendBuffer(packet);
  return send_buffer;
}
