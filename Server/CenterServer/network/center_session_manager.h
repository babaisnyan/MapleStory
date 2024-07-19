#pragma once

struct ServerInfo;
class CenterSession;

using CenterSessionRef = std::shared_ptr<CenterSession>;

class CenterSessionManager {
public:
  static CenterSessionManager& GetInstance() {
    static CenterSessionManager instance;
    return instance;
  }

public:
  void Add(const CenterSessionRef& session, const String& name);
  void Remove(const CenterSessionRef& session);
  void RemoveByName(const String& name);

  bool IsExist(const String& name);
  bool IsExist(const String& ip, uint16_t port);
  CenterSessionRef Get(const String& name);

  void Broadcast(const SendBufferRef& send_buffer);

  std::optional<std::shared_ptr<ServerInfo>> GetServerInfo(const String& name);

private:
  USE_LOCK;
  Map<String, CenterSessionRef> _center_sessions;
  HashMap<String, std::shared_ptr<ServerInfo>> _server_infos;
};
