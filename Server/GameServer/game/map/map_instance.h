﻿#pragma once

#include <google/protobuf/message.h>

#include "data/templates/map_template.h"

#include "job/job_queue.h"

#include "network/game/game_client_packet_handler.h"
#include "network/game/game_session.h"

namespace game {
  class Monster;
  class MobTemplate;
  class GameObject;

  class MapInstance final : public JobQueue {
  public:
    explicit MapInstance(const int32_t map_id, std::pair<int32_t, int32_t> size, std::vector<GroundInfo> grounds);
    ~MapInstance() = default;

  public:
    /* DoAsync를 통해서 호출 */
    bool AddPlayer(const std::shared_ptr<GameSession>& session);
    bool RemoveObject(const int64_t object_id);
    void MovePlayer(const std::shared_ptr<GameSession>& session, const protocol::GameClientPlayerMove& packet);
    void Update(float delta);

    std::optional<std::shared_ptr<GameSession>> GetPlayer(int64_t object_id) const;
    std::optional<std::shared_ptr<GameSession>> GetPlayer(int32_t player_id) const;
    std::unordered_map<int32_t, std::shared_ptr<GameSession>> GetPlayers() const;

    template <typename T> requires std::is_base_of_v<google::protobuf::Message, T>
    void BroadCast(const T& message, const std::shared_ptr<GameSession> self_session);

    template <typename T> requires std::is_base_of_v<google::protobuf::Message, T>
    void BroadCast(const T& message, const int32_t self_player_id);

  private:
    void OnPlayerEnter(const std::shared_ptr<GameSession>& session);
    void OnPlayerLeave(const std::shared_ptr<Player>& player);
    void RespawnMobs();

  public:
    int32_t GetMapId() const noexcept;
    std::pair<int32_t, int32_t> GetSize() const noexcept;

    void AddSpawnLocation(const std::shared_ptr<SpawnPoint>& spawn_point, const std::shared_ptr<MobTemplate>& mob_template);

  private:
    const int32_t _map_id;
    const std::pair<int32_t, int32_t> _size;
    const std::vector<GroundInfo> _grounds;
    uint64_t _last_respawn_tick = 0;

    std::unordered_map<std::shared_ptr<SpawnPoint>, std::shared_ptr<MobTemplate>> _mob_spawn_locations;
    std::unordered_map<std::shared_ptr<SpawnPoint>, std::shared_ptr<Monster>> _mobs;
    std::unordered_map<int64_t, std::shared_ptr<GameObject>> _objects;
    std::unordered_map<int32_t, std::shared_ptr<GameSession>> _players;
    std::vector<std::vector<std::vector<std::shared_ptr<GameObject>>>> _grid;
  };

  template <typename T> requires std::is_base_of_v<google::protobuf::Message, T>
  void MapInstance::BroadCast(const T& message, const std::shared_ptr<GameSession> self_session) {
    const auto buffer = GameClientPacketHandler::MakeSendBuffer(message);

    for (const auto& session : _players | std::views::values) {
      if (session == self_session) {
        continue;
      }

      session->Send(buffer);
    }
  }

  template <typename T> requires std::is_base_of_v<google::protobuf::Message, T>
  void MapInstance::BroadCast(const T& message, const int32_t self_player_id) {
    const auto buffer = GameClientPacketHandler::MakeSendBuffer(message);

    for (const auto& [player_id, session] : _players) {
      if (player_id == self_player_id) {
        continue;
      }

      session->Send(buffer);
    }
  }
}
