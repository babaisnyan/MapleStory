#include "pch.h"
#include "game_tick.h"

#include "game/player_manager.h"
#include "game/player_stat.h"
#include "game/map/map_instance.h"
#include "game/map/map_manager.h"
#include "game/objects/player/player.h"

#include "job/job.h"

#include "network/game/game_client_packet_handler.h"
#include "network/game/game_session_manager.h"

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