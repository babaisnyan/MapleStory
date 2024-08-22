#include "pch.h"
#include "player_handler.h"

#include "game/objects/player/key_map.h"
#include "game/objects/player/player.h"

#include "network/game/game_session.h"

void PlayerHandler::HandleKeySettingChange(const GameSessionRef& session, const protocol::GameClientChangeKeySetting& packet) {
  const auto player = session->GetPlayer();

  if (!player) {
    return;
  }

  const auto key_map = player->GetKeyMap();

  if (!key_map) {
    return;
  }

  switch (packet.key_setting().key_type()) {
    case protocol::KEY_TYPE_ITEM:
      key_map->SetItem(packet.key_setting().key_code(), packet.key_setting().item_id());
      break;
    case protocol::KEY_TYPE_SKILL:
      key_map->SetSkill(packet.key_setting().key_code(), packet.key_setting().skill_id());
      break;
    default:
      key_map->SetKey(packet.key_setting().key_code(), packet.key_setting().key_type());
      break;
  }
}
