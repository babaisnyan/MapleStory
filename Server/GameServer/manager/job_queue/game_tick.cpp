#include "pch.h"
#include "game_tick.h"

#include "game/map/map_instance.h"
#include "game/map/map_manager.h"

#include "handler/game/player_handler.h"

#include "job/job.h"

#include "thread/thread_manager.h"

#include "utils/timer/timer_manager.h"

void GameTick::Start() {
  ThreadManager::GetInstance().Launch([self = shared_from_this()] {
    auto& timer_manager = TimerManager::GetInstance();

    ASSERT_CRASH(timer_manager.AddTimer(L"WorldTick"));

    const auto maps = MapManager::GetInstance().GetAllMapInstances() | std::views::values;
    float time = 0.0f;

    while (self->_is_running) {
      const auto delta = timer_manager.GetDeltaTime(L"WorldTick");
      time += delta;

      if (time > 0.2f) {
        for (const auto& map_instance : maps) {
          map_instance->DoAsync([&map_instance, time] {
            map_instance->Update(time);
          });
        }

        time = 0.0f;
      }

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

void GameTick::HandleKeySettingChange(const GameSessionRef& session, const protocol::GameClientChangeKeySetting& packet) {
  PlayerHandler::HandleKeySettingChange(session, packet);
}
