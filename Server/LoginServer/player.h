#pragma once

class Player {
public:
  uint64_t GetId() const { return _id; }
  void     SetId(const uint64_t id) { _id = id; }

  const std::string& GetName() const { return _name; }
  void               SetName(const std::string& name) { _name = name; }

  protocol::PlayerType GetType() const { return _type; }
  void                 SetType(const protocol::PlayerType type) { _type = type; }

  GameSessionRef GetSession() const { return _session; }
  void           SetSession(const GameSessionRef& session) { _session = session; }

private:
  uint64_t             _id;
  std::string          _name;
  protocol::PlayerType _type = protocol::PLAYER_TYPE_NONE;

  GameSessionRef _session;
};
