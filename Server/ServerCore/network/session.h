#pragma once
#include "iocp_core.h"
#include "iocp_event.h"
#include "network_address.h"
#include "recv_buffer.h"

class Service;

class Session : public IocpObject {
  friend class Listener;
  friend class IocpCore;
  friend class Service;

  enum {
    kBufferSize = 0x10000
  };

public:
  Session();
  virtual ~Session();

public:
  void Send(const SendBufferRef& send_buffer);
  bool Connect();
  void Disconnect(const WCHAR* cause);

  std::shared_ptr<Service> GetService() const noexcept { return _service.lock(); }
  void                     SetService(const std::shared_ptr<Service>& service) noexcept { _service = service; }

public:
  NetworkAddress& GetNetworkAddress() noexcept { return _network_address; }
  void            SetNetworkAddress(const NetworkAddress network_address) noexcept { _network_address = network_address; }
  SOCKET          GetSocket() const noexcept { return _socket; }
  bool            IsConnected() const noexcept { return _connected; }
  SessionRef      GetSession() noexcept { return std::static_pointer_cast<Session>(shared_from_this()); }

private:
  HANDLE GetHandle() const noexcept override;
  void   Dispatch(IocpEvent* event, int32_t num_of_bytes) override;

private:
  bool RegisterConnect();
  bool RegisterDisconnect();
  void RegisterRecv();
  void RegisterSend();

  void ProcessConnect();
  void ProcessDisconnect();
  void ProcessRecv(int32_t num_of_bytes);
  void ProcessSend(int32_t num_of_bytes);

  void HandleError(int32_t error_code);

protected:
  virtual void    OnConnected() {}
  virtual void    OnDisconnected() {}
  virtual int32_t OnRecv(std::byte* buffer, const int32_t len) { return len; }
  virtual void    OnSend(const int32_t len) {}

private:
  std::weak_ptr<Service> _service;
  SOCKET                 _socket = INVALID_SOCKET;
  NetworkAddress         _network_address = {};
  Atomic<bool>           _connected = false;

private:
  USE_LOCK;
  RecvBuffer _recv_buffer;

  Queue<SendBufferRef> _send_queue;
  Atomic<bool>         _send_registered = false;

private:
  ConnectEvent    _connect_event;
  DisconnectEvent _disconnect_event;
  SendEvent       _send_event;
  RecvEvent       _recv_event;
};

struct PacketHeader {
  uint16_t size;
  uint16_t id;
};

class PacketSession : public Session {
public:
  PacketSession() = default;
  virtual ~PacketSession() = default;

  PacketSessionRef GetPacketSession() noexcept {
    return std::static_pointer_cast<PacketSession>(shared_from_this());
  }

protected:
  int32_t      OnRecv(std::byte* buffer, const int32_t len) final;
  virtual void OnRecvPacket(std::byte* buffer, const int32_t len) = 0;
};
