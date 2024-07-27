#include "pch.h"
#include "game_packet_creator.h"
#include "game_client_packet_handler.h"
#include "network/protocol/game_protocol.pb.h"

SendBufferRef GamePacketCreator::GetClientEnterResponse(const bool success) {
  protocol::GameServerEnter packet;
  packet.set_success(success);

  const auto send_buffer = GameClientPacketHandler::MakeSendBuffer(packet);
  return send_buffer;
}
