#include "pch.h"
#include "game_packet_creator.h"
#include "game_client_packet_handler.h"

#include "game/player.h"

#include "network/protocol/game_protocol.pb.h"

SendBufferRef GamePacketCreator::GetClientEnterFailResponse() {
  protocol::GameServerEnter packet;
  packet.set_success(false);

  const auto send_buffer = GameClientPacketHandler::MakeSendBuffer(packet);
  return send_buffer;
}

SendBufferRef GamePacketCreator::GetClientEnterSuccessResponse(const std::shared_ptr<Player>& player) {
  protocol::GameServerEnter packet;
  packet.set_success(true);
  packet.set_map_id(player->GetMap());

  const auto name = utils::ConvertToUtf8(player->GetName());

  if (name.has_value()) {
    const auto player_info = packet.mutable_player_info();

    player_info->set_id(player->GetId());
    player_info->set_name(name.value());
    player_info->set_type(player->GetType());
    player_info->set_level(player->GetLevel());
    player_info->set_exp(player->GetExp());
    player_info->set_meso(player->GetMeso());
    player_info->set_hp(player->GetHp());
    player_info->set_mp(player->GetMp());
    player_info->set_max_hp(player->GetMaxHp());
    player_info->set_max_mp(player->GetMaxMp());
    player_info->set_str(player->GetStr());
    player_info->set_dex(player->GetDex());
    player_info->set_int_(player->GetInt());
    player_info->set_luk(player->GetLuk());
  }

  const auto send_buffer = GameClientPacketHandler::MakeSendBuffer(packet);
  return send_buffer;
}
