#pragma once

#include <functional>

#include "network_address.h"

class Listener;

enum class ServiceType {
  kServer,
  kClient
};

using SessionFactory = std::function<SessionRef()>;

class Service : public std::enable_shared_from_this<Service> {
public:
  Service(ServiceType type, NetworkAddress address, IocpCoreRef core, SessionFactory factory, int32_t max_session_count = 1);
  virtual ~Service();

  virtual bool Start() = 0;
  bool         CanStart() const noexcept { return _factory != nullptr; }

  virtual void Close();
  void         SetFactory(SessionFactory factory) { _factory = std::move(factory); }

  void       Broadcast(const SendBufferRef& send_buffer);
  SessionRef CreateSession();
  void       AddSession(SessionRef session);
  void       ReleaseSession(const SessionRef& session);
  int32_t    GetSessionCount() const noexcept { return _session_count; }
  int32_t    GetMaxSessionCount() const noexcept { return _max_session_count; }

public:
  ServiceType     GetType() const noexcept { return _type; }
  NetworkAddress& GetAddress() noexcept { return _address; }
  IocpCoreRef&    GetCore() noexcept { return _core; }

protected:
  USE_LOCK;

  ServiceType    _type;
  NetworkAddress _address;
  IocpCoreRef    _core;

  Set<SessionRef> _sessions;
  int32_t         _session_count = 0;
  int32_t         _max_session_count;
  SessionFactory  _factory;
};

class ClientService : public Service {
public:
  ClientService(NetworkAddress address, IocpCoreRef core, SessionFactory factory, int32_t max_session_count = 1);
  virtual ~ClientService();

  bool Start() override;
  void Close() override;
};

class ServerService : public Service {
public:
  ServerService(NetworkAddress address, IocpCoreRef core, SessionFactory factory, int32_t max_session_count = 1);
  virtual ~ServerService();

  bool Start() override;
  void Close() override;

private:
  ListenerRef _listener = nullptr;
};
