#pragma once

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
    explicit MapInstance(const int32_t map_id, std::pair<int32_t, int32_t> size, std::vector<GroundInfo> grounds, std::tuple<int32_t, int32_t, int32_t, int32_t> bounds);
    ~MapInstance() = default;

  public:
    /* DoAsync를 통해서 호출 */
    bool AddPlayer(const std::shared_ptr<GameSession>& session);
    bool RemoveObject(const int64_t object_id);
    void MovePlayer(const std::shared_ptr<GameSession>& session, const protocol::GameClientPlayerMove& packet);
    void MoveObject(const std::shared_ptr<GameObject>& object, const int16_t old_x, int16_t old_y);
    void Update(float delta);

    void OnChangeKeySetting(const std::shared_ptr<GameSession>& session, const protocol::GameClientChangeKeySetting& packet);
    void OnChat(const std::shared_ptr<GameSession>& session, const protocol::GameClientChat& packet);
    void OnRevive(const std::shared_ptr<GameSession>& session, const std::shared_ptr<Player>& player);
    void OnAttack(const std::shared_ptr<GameSession>& session, const std::shared_ptr<Player>& player, const protocol::GameClientAttack& packet);
    void OnMoveItem(const std::shared_ptr<Player>& player, const protocol::GameClientMoveInventory& packet);
    void OnUseItem(const std::shared_ptr<Player>& player, const protocol::GameClientUseItem& packet);

    void NotifyPlayerDamage(int32_t damage, int64_t object_id);
    void NotifyPlayerDeath(int64_t object_id);
    void NotifyPlayerLevelUp(int64_t object_id, int32_t level);

    std::optional<std::shared_ptr<GameSession>> GetPlayer(int64_t object_id) const;
    std::optional<std::shared_ptr<GameSession>> GetPlayer(int32_t player_id) const;
    std::unordered_map<int32_t, std::shared_ptr<GameSession>> GetPlayers() const;

    template <typename T> requires std::is_base_of_v<google::protobuf::Message, T>
    void BroadCast(const T& message, const std::shared_ptr<GameSession> self_session);

    template <typename T> requires std::is_base_of_v<google::protobuf::Message, T>
    void BroadCast(const T& message, const int32_t self_player_id);

    template <typename T> requires std::is_base_of_v<google::protobuf::Message, T>
    void Send(const T& message, const int32_t self_player_id);

  private:
    void OnPlayerEnter(const std::shared_ptr<GameSession>& session);
    void OnPlayerLeave(const std::shared_ptr<Player>& player);
    void RespawnMobs();

  public:
    int32_t GetMapId() const noexcept;
    std::pair<int32_t, int32_t> GetSize() const noexcept;
    std::tuple<int32_t, int32_t, int32_t, int32_t> GetBounds() const noexcept;
    std::pair<int32_t, int32_t> GetGridSize() const noexcept;
    const std::vector<std::shared_ptr<GameObject>>& GetObjects(int16_t x, int16_t y) noexcept;

    void AddSpawnLocation(const std::shared_ptr<SpawnPoint>& spawn_point, const std::shared_ptr<MobTemplate>& mob_template);

  private:
    const int32_t _map_id;
    const std::pair<int32_t, int32_t> _size;
    const std::tuple<int32_t, int32_t, int32_t, int32_t> _bounds;
    const std::vector<GroundInfo> _grounds;
    int32_t _max_grid_x;
    int32_t _max_grid_y;
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

  template <typename T> requires std::is_base_of_v<google::protobuf::Message, T>
  void MapInstance::Send(const T& message, const int32_t self_player_id) {
    const auto buffer = GameClientPacketHandler::MakeSendBuffer(message);

    if (const auto session = GetPlayer(self_player_id); session.has_value()) {
      session.value()->Send(buffer);
    }
  }
}
