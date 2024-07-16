#pragma once

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

private:
  USE_LOCK;
  Map<String, CenterSessionRef> _center_sessions;
};
