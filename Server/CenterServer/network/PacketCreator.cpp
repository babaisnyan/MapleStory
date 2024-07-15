#include "pch.h"
#include "PacketCreator.h"

#include "center_client_packet_handler.h"

#include "protocol/center_protocol.pb.h"

using namespace protocol;

SendBufferRef PacketCreator::GetHandshake() {
  CenterServerHandshake packet;
  packet.set_version(constants::kVersion);
  return CenterClientPacketHandler::MakeSendBuffer(packet);
}
