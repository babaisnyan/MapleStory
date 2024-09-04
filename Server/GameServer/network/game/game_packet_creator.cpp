#include "pch.h"
#include "game_packet_creator.h"

#include "game_client_packet_handler.h"

#include "game/player_stat.h"
#include "game/objects/player/key_map.h"
#include "game/objects/player/player.h"

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

    player_info->set_object_id(player->GetObjectId());
    player_info->set_id(player->GetId());
    player_info->set_name(name.value());
    player_info->set_type(player->GetType());
    player_info->set_level(player->GetStat()->GetLevel());
    player_info->set_exp(player->GetStat()->GetExp());
    player_info->set_meso(player->GetMeso());
    player_info->set_hp(player->GetStat()->GetHp());
    player_info->set_mp(player->GetStat()->GetMp());
    player_info->set_max_hp(player->GetStat()->GetMaxHp());
    player_info->set_max_mp(player->GetStat()->GetMaxMp());
    player_info->set_str(player->GetStat()->GetStr());
    player_info->set_dex(player->GetStat()->GetDex());
    player_info->set_int_(player->GetStat()->GetInt());
    player_info->set_luk(player->GetStat()->GetLuk());
    player_info->set_x(player->GetPosition().x);
    player_info->set_y(player->GetPosition().y);
    player_info->set_ap(player->GetStat()->GetAp());
    player_info->set_sp(player->GetStat()->GetSp());

    const auto key_map = player->GetKeyMap()->GetAll();

    for (const auto& [code, entry] : key_map) {
      const auto key = player_info->add_key_settings();
      key->set_key_code(code);
      key->set_key_type(entry->type);

      if (entry->type == protocol::KEY_TYPE_ITEM) {
        key->set_item_id(entry->data);
      } else if (entry->type == protocol::KEY_TYPE_SKILL) {
        key->set_skill_id(entry->data);
      }
    }
  }

  const auto send_buffer = GameClientPacketHandler::MakeSendBuffer(packet);
  return send_buffer;
}
