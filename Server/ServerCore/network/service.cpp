#include "pch.h"
#include "service.h"

#include "iocp_core.h"
#include "listener.h"
#include "session.h"

#include "memory/memory.h"

Service::Service(const ServiceType type, const NetworkAddress address, IocpCoreRef core, SessionFactory factory, const int32_t max_session_count)
  : _type(type), _address(address), _core(std::move(core)), _max_session_count(max_session_count), _factory(std::move(factory)) {}

Service::~Service() {}

void Service::Close() {}

void Service::Broadcast(const SendBufferRef& send_buffer) {
  WRITE_LOCK;

  for (const auto& session : _sessions) {
    session->Send(send_buffer);
  }
}

SessionRef Service::CreateSession() {
  SessionRef session = _factory();
  session->SetService(shared_from_this());

  if (!_core->Register(session)) {
    return nullptr;
  }

  return session;
}

void Service::AddSession(SessionRef session) {
  WRITE_LOCK;

  _session_count++;
  _sessions.insert(std::move(session));
}

void Service::ReleaseSession(const SessionRef& session) {
  WRITE_LOCK;
  ASSERT_CRASH(_sessions.erase(session) != 0);

  _session_count--;
}

ClientService::ClientService(const NetworkAddress address, IocpCoreRef core, SessionFactory factory, const int32_t max_session_count)
  : Service(ServiceType::kClient, address, std::move(core), std::move(factory), max_session_count) {}

ClientService::~ClientService() {}

bool ClientService::Start() {
  if (!CanStart()) {
    return false;
  }

  const int32_t session_count = GetMaxSessionCount();

  for (int i = 0; i < session_count; i++) {
    const SessionRef session = CreateSession();

    if (!session->Connect()) {
      return false;
    }
  }

  return true;
}

void ClientService::Close() {
  Service::Close();
}

ServerService::ServerService(const NetworkAddress address, IocpCoreRef core, SessionFactory factory, const int32_t max_session_count)
  : Service(ServiceType::kServer, address, std::move(core), std::move(factory), max_session_count) {}

ServerService::~ServerService() {}

bool ServerService::Start() {
  if (!CanStart()) {
    return false;
  }

  _listener = MakeShared<Listener>();

  if (_listener == nullptr) {
    return false;
  }

  const ServerServiceRef service = std::static_pointer_cast<ServerService>(shared_from_this());

  if (!_listener->Start(service)) {
    return false;
  }

  return true;
}

void ServerService::Close() {
  Service::Close();
}
