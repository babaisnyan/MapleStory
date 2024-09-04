#include "pch.h"
#include "listener.h"

#include "iocp_event.h"
#include "network_address.h"
#include "service.h"
#include "session.h"
#include "socket_utils.h"

#include "memory/memory.h"

Listener::~Listener() {
  SocketUtils::Close(_socket);

  for (AcceptEvent* event : _accept_events) {
    MsDelete(event);
  }
}

bool Listener::Start(const ServerServiceRef& service) {
  _service = service;

  if (_service == nullptr) {
    return false;
  }

  _socket = SocketUtils::CreateSocket();

  if (_socket == INVALID_SOCKET) {
    return false;
  }

  if (!_service->GetCore()->Register(shared_from_this())) {
    return false;
  }

  if (!SocketUtils::SetReuseAddress(_socket, true)) {
    return false;
  }

  if (!SocketUtils::SetLinger(_socket, 0, 0)) {
    return false;
  }

  if (!SocketUtils::Bind(_socket, _service->GetAddress())) {
    return false;
  }

  if (!SocketUtils::Listen(_socket)) {
    return false;
  }

  const int32_t accept_count = _service->GetMaxSessionCount();

  for (int i = 0; i < accept_count; i++) {
    AcceptEvent* accept_event = MsNew<AcceptEvent>();
    accept_event->SetOwner(shared_from_this());

    _accept_events.push_back(accept_event);
    RegisterAccept(accept_event);
  }

  return true;
}

void Listener::Close() {
  SocketUtils::Close(_socket);
}

HANDLE Listener::GetHandle() const noexcept {
  return reinterpret_cast<HANDLE>(_socket);
}

void Listener::Dispatch(IocpEvent* event, int32_t num_of_bytes) {
  ASSERT_CRASH(event->GetEventType() == IocpEvent::EventType::kAccept);

  AcceptEvent* accept_event = static_cast<AcceptEvent*>(event);
  ProcessAccept(accept_event);
}

void Listener::RegisterAccept(AcceptEvent* event) {
  const SessionRef session = _service->CreateSession();

  event->Init();
  event->_session = session;

  DWORD      bytes_received = 0;
  const bool result = SocketUtils::AcceptEx(_socket,
                                            session->GetSocket(),
                                            session->_recv_buffer.GetWritePos(),
                                            0,
                                            sizeof(SOCKADDR_IN) + 16,
                                            sizeof(SOCKADDR_IN) + 16,
                                            &bytes_received,
                                            static_cast<OVERLAPPED*>(event));

  if (!result) {
    const int32_t error_code = WSAGetLastError();

    if (error_code != WSA_IO_PENDING) {
      RegisterAccept(event);
    }
  }
}

void Listener::ProcessAccept(AcceptEvent* event) {
  const SessionRef session = event->_session;

  if (!SocketUtils::SetUpdateAcceptSocket(session->GetSocket(), _socket)) {
    RegisterAccept(event);
    return;
  }

  SOCKADDR_IN socket_address;
  int32_t     socket_address_size = sizeof(socket_address);

  const int32_t peer_name_result = getpeername(session->GetSocket(),
                                               reinterpret_cast<SOCKADDR*>(&socket_address),
                                               &socket_address_size);

  if (peer_name_result == SOCKET_ERROR) {
    RegisterAccept(event);
    return;
  }

  session->SetNetworkAddress(NetworkAddress(socket_address));
  session->ProcessConnect();

  RegisterAccept(event);
}
