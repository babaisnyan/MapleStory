#pragma once

namespace center {
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
    void Add(const CenterSessionRef& session);
    void Remove(const String& name);

    bool IsExist(const String& name) const;
    bool IsExist(const String& ip, uint16_t port);
    std::optional<CenterSessionRef> Get(const String& name) const;

    void Broadcast(const SendBufferRef& send_buffer);

  private:
    tbb::concurrent_hash_map<String, CenterSessionRef> _center_sessions;
  };
}
