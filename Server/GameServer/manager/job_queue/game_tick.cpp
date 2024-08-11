#include "pch.h"
#include "game_tick.h"

#include "game/player_stat.h"
#include "game/map/map_instance.h"
#include "game/objects/player/player.h"

#include "job/job.h"

#include "network/game/game_client_packet_handler.h"

#include "thread/thread_manager.h"

void GameTick::Start() {
  ThreadManager::GetInstance().Launch([self = shared_from_this()] {
    while (self->_is_running) {
      //TODO: Tick World



      if (!self->_job_queue.empty()) {
        JobRef job;
        const uint64_t current_tick = GetTickCount64();

        while (self->_job_queue.try_pop(job)) {
          job->Execute();

          if (current_tick + 100 < GetTickCount64()) {
            break;
          }
        }
      }
    }
  });
}

void GameTick::AddPlayer(const std::shared_ptr<MapInstance>& map, const GameSessionRef& session) {
  protocol::GameServerAddPlayer add_players;
  for (auto&& other_session : map->GetPlayers() | std::views::values) {
    const auto info = other_session->GetPlayer();
    const auto player_info = add_players.add_player_infos();
    player_info->set_object_id(info->GetObjectId());
    player_info->set_name(utils::ConvertToUtf8(info->GetName()).value());
    player_info->set_type(info->GetType());
    player_info->set_level(info->GetStat()->GetLevel());
    player_info->set_hp(info->GetStat()->GetHp());
    player_info->set_max_hp(info->GetStat()->GetMaxHp());
    player_info->set_x(static_cast<int32_t>(info->GetPosition().x));
    player_info->set_y(static_cast<int32_t>(info->GetPosition().y));
  }

  if (add_players.player_infos_size() > 0) {
    const auto send_buffer = GameClientPacketHandler::MakeSendBuffer(add_players);
    session->Send(send_buffer);
  }

  const auto player = session->GetPlayer();

  if (map->AddPlayer(session)) {
    protocol::GameServerAddPlayer add_player;
    const auto info = add_player.add_player_infos();
    info->set_object_id(player->GetObjectId());
    info->set_name(utils::ConvertToUtf8(player->GetName()).value());
    info->set_type(player->GetType());
    info->set_level(player->GetStat()->GetLevel());
    info->set_hp(player->GetStat()->GetHp());
    info->set_max_hp(player->GetStat()->GetMaxHp());
    info->set_x(static_cast<int32_t>(player->GetPosition().x));
    info->set_y(static_cast<int32_t>(player->GetPosition().y));
    map->BroadCast(add_player, player->GetId());
  }
}

void GameTick::RemovePlayer(const std::shared_ptr<MapInstance>& map, const int64_t object_id) {
  map->RemoveObject(object_id);
}
