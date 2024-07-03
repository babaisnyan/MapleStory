#pragma once
#include "network/session.h"

class GameSession : public PacketSession {
public:
  ~GameSession() override {
    std::cout << "GameSession Destructor\n";
  }

public:
  PlayerRef GetPlayer(const uint64_t index) {
    return _players[index];
  }

  PlayerRef GetPlayer() const {
    return _player;
  }

  void SetPlayer(const PlayerRef& player) {
    _player = player;
  }

  void AddPlayer(const PlayerRef& player) {
    _players.push_back(player);
  }

  void RemovePlayer(const PlayerRef& player) {
    std::erase(_players, player);
  }

  std::weak_ptr<class Room> GetRoom() const {
    return _room;
  }

  void SetRoom(const std::weak_ptr<class Room>& room) {
    _room = room;
  }

protected:
  void OnConnected() override;
  void OnDisconnected() override;
  void OnRecvPacket(std::byte* buffer, const int32_t len) override;
  void OnSend(const int32_t len) override;

private:
  Vector<PlayerRef> _players;

  PlayerRef                 _player;
  std::weak_ptr<class Room> _room;
};
