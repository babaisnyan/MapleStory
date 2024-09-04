#pragma once

#include "iocp_core.h"

class AcceptEvent;
class NetworkAddress;

class Listener : public IocpObject {
public:
  ~Listener();

public:
  bool Start(const ServerServiceRef& service);
  void Close();

public:
  HANDLE GetHandle() const noexcept override;
  void Dispatch(IocpEvent* event, int32_t num_of_bytes) override;

private:
  void RegisterAccept(AcceptEvent* event);
  void ProcessAccept(AcceptEvent* event);

protected:
  SOCKET _socket = INVALID_SOCKET;
  Vector<AcceptEvent*> _accept_events;
  ServerServiceRef _service;
};
