#pragma once

#include "types.h"

class Session;

class IocpEvent : public OVERLAPPED {
public:
  enum class EventType : uint8_t {
    kConnect,
    kDisconnect,
    kAccept,
    kRecv,
    kSend
  };

public:
  explicit IocpEvent(EventType type);

  void Init();

public:
  EventType& GetEventType() noexcept {
    return _event_type;
  }

  IocpObjectRef& GetOwner() noexcept {
    return _owner;
  }

  void SetOwner(IocpObjectRef owner) {
    _owner = std::move(owner);
  }

protected:
  EventType     _event_type;
  IocpObjectRef _owner;
};

class ConnectEvent : public IocpEvent {
public:
  ConnectEvent() : IocpEvent(EventType::kConnect) {}
};

class DisconnectEvent : public IocpEvent {
public:
  DisconnectEvent() : IocpEvent(EventType::kDisconnect) {}
};

class AcceptEvent : public IocpEvent {
public:
  AcceptEvent() : IocpEvent(EventType::kAccept) {}

public:
  SessionRef _session = nullptr;
};

class RecvEvent : public IocpEvent {
public:
  RecvEvent() : IocpEvent(EventType::kRecv) {}
};

class SendEvent : public IocpEvent {
public:
  SendEvent() : IocpEvent(EventType::kSend) {}

public:
  int32_t GetSendBufferCount() const noexcept {
    return static_cast<int32_t>(_send_buffers.size());
  }

  const Vector<SendBufferRef>& GetSendBuffers() noexcept {
    return _send_buffers;
  }

  void AddBuffer(const SendBufferRef& send_buffer) noexcept {
    _send_buffers.push_back(send_buffer);
  }

  void ClearBuffers() noexcept {
    _send_buffers.clear();
  }

private:
  Vector<SendBufferRef> _send_buffers;
};
