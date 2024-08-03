#pragma once
#include <google/protobuf/message.h>

#include "network/game/game_client_packet_handler.h"
#include "network/game/game_session.h"

namespace game {
  class GameObject;

  class MapInstance final : public std::enable_shared_from_this<MapInstance> {
  public:
    explicit MapInstance(const int32_t map_id);
    ~MapInstance() = default;

  public:
    bool AddPlayer(const std::shared_ptr<GameSession>& session);
    bool RemovePlayer(int32_t player_id);
    std::optional<std::shared_ptr<GameSession>> GetPlayer(int32_t player_id) const;

    template <typename T> requires std::is_base_of_v<google::protobuf::Message, T>
    void BroadCast(T message, const int32_t self_player_id);

  public:
    int32_t GetMapId() const noexcept;

  private:
    const int32_t _map_id;

    std::unordered_map<int64_t, std::shared_ptr<GameObject>> _objects;
    std::unordered_map<int32_t, std::shared_ptr<GameSession>> _players;
  };

  template <typename T> requires std::is_base_of_v<google::protobuf::Message, T>
  void MapInstance::BroadCast(T message, const int32_t self_player_id) {
    const auto buffer = GameClientPacketHandler::MakeSendBuffer(message);

    for (const auto& [player_id, session] : _players) {
      if (player_id == self_player_id) {
        continue;
      }

      session->Send(buffer);
    }
  }
}
