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
  void Add(const CenterSessionRef& session);
  void Remove(const CenterSessionRef& session);

  void Broadcast(const SendBufferRef& send_buffer);

private:
  USE_LOCK;
  Set<CenterSessionRef> _center_sessions;
};