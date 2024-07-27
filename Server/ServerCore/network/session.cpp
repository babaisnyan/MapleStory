#include "pch.h"
#include "session.h"

#include "service.h"
#include "socket_utils.h"

Session::Session() : _recv_buffer(kBufferSize) {
  _socket = SocketUtils::CreateSocket();
}

Session::~Session() {
  SocketUtils::Close(_socket);
}

void Session::Send(const SendBufferRef& send_buffer) {
  if (!IsConnected()) {
    return;
  }

  bool register_send = false;

  {
    WRITE_LOCK;

    _send_queue.push(send_buffer);

    if (!_send_registered.exchange(true)) {
      register_send = true;
    }
  }

  if (register_send) {
    RegisterSend();
  }
}

bool Session::Connect() {
  return RegisterConnect();
}

void Session::Disconnect(const WCHAR* cause) {
  if (!_connected.exchange(false)) { return; }

  std::wcout << "Disconnect : " << cause << '\n';

  RegisterDisconnect();
}

HANDLE Session::GetHandle() const noexcept { return reinterpret_cast<HANDLE>(_socket); }

void Session::Dispatch(IocpEvent* event, const int32_t num_of_bytes) {
  switch (event->GetEventType()) {
    case IocpEvent::EventType::kConnect:
      ProcessConnect();
      break;
    case IocpEvent::EventType::kDisconnect:
      ProcessDisconnect();
      break;
    case IocpEvent::EventType::kRecv:
      ProcessRecv(num_of_bytes);
      break;
    case IocpEvent::EventType::kSend:
      ProcessSend(num_of_bytes);
      break;
    default:
      break;
  }
}

bool Session::RegisterConnect() {
  if (IsConnected()) {
    return false;
  }

  if (GetService()->GetType() != ServiceType::kClient) {
    return false;
  }

  if (!SocketUtils::SetReuseAddress(_socket, true)) {
    return false;
  }

  if (!SocketUtils::BindAnyAddress(_socket, 0)) {
    return false;
  }

  _connect_event.Init();
  _connect_event.SetOwner(shared_from_this());

  DWORD       num_of_bytes;
  SOCKADDR_IN socket_address = GetService()->GetAddress().GetSocketAddress();

  const bool result = SocketUtils::ConnectEx(_socket,
                                             reinterpret_cast<SOCKADDR*>(&socket_address),
                                             sizeof(socket_address),
                                             nullptr,
                                             0,
                                             &num_of_bytes,
                                             &_connect_event);

  if (!result) {
    const int32_t error_code = WSAGetLastError();

    if (error_code != WSA_IO_PENDING) {
      _connect_event.SetOwner(nullptr);
      return false;
    }
  }

  return true;
}

bool Session::RegisterDisconnect() {
  _disconnect_event.Init();
  _disconnect_event.SetOwner(shared_from_this());

  const bool result = SocketUtils::DisconnectEx(_socket,
                                                &_disconnect_event,
                                                TF_REUSE_SOCKET,
                                                0);

  if (!result) {
    const int32_t error_code = WSAGetLastError();

    if (error_code != WSA_IO_PENDING) {
      HandleError(error_code);
      _recv_event.SetOwner(nullptr);
    }
  }

  return true;
}

void Session::RegisterRecv() {
  if (!IsConnected()) {
    return;
  }

  _recv_event.Init();
  _recv_event.SetOwner(shared_from_this());

  WSABUF buffer = {
    static_cast<unsigned long>(_recv_buffer.GetFreeSize()),
    reinterpret_cast<char*>(_recv_buffer.GetWritePos())
  };
  DWORD num_of_bytes = 0;
  DWORD flags = 0;

  const int32_t result = WSARecv(_socket,
                                 &buffer,
                                 1,
                                 &num_of_bytes,
                                 &flags,
                                 &_recv_event,
                                 nullptr);

  if (result == SOCKET_ERROR) {
    const int32_t error_code = WSAGetLastError();

    if (error_code != WSA_IO_PENDING) {
      HandleError(error_code);
      _recv_event.SetOwner(nullptr);
    }
  }
}

void Session::RegisterSend() {
  if (!IsConnected()) {
    return;
  }

  _send_event.Init();
  _send_event.SetOwner(shared_from_this());

  {
    WRITE_LOCK;

    int32_t write_size = 0;

    while (!_send_queue.empty()) {
      const SendBufferRef send_buffer = _send_queue.front();
      _send_queue.pop();

      write_size += send_buffer->GetWriteSize();

      _send_event.AddBuffer(send_buffer);
    }
  }

  Vector<WSABUF> wsa_buffers;
  wsa_buffers.reserve(_send_event.GetSendBufferCount());

  for (const SendBufferRef& send_buffer : _send_event.GetSendBuffers()) {
    wsa_buffers.emplace_back(send_buffer->GetWriteSize(),
                             reinterpret_cast<char*>(send_buffer->GetBuffer()));
  }


  DWORD         num_of_bytes = 0;
  const int32_t result = WSASend(_socket,
                                 wsa_buffers.data(),
                                 static_cast<uint32_t>(wsa_buffers.size()),
                                 &num_of_bytes,
                                 0,
                                 &_send_event,
                                 nullptr);

  if (result == SOCKET_ERROR) {
    const int32_t error_code = WSAGetLastError();

    if (error_code != WSA_IO_PENDING) {
      HandleError(error_code);
      _send_event.SetOwner(nullptr);
      _send_event.ClearBuffers();
      _send_registered.store(false);
    }
  }
}

void Session::ProcessConnect() {
  _connect_event.SetOwner(nullptr);
  _connected.store(true);

  GetService()->AddSession(GetSession());

  OnConnected();

  RegisterRecv();
}

void Session::ProcessDisconnect() {
  _disconnect_event.SetOwner(nullptr);

  OnDisconnected();
  GetService()->ReleaseSession(GetSession());
}

void Session::ProcessRecv(const int32_t num_of_bytes) {
  _recv_event.SetOwner(nullptr);

  if (num_of_bytes == 0) {
    Disconnect(L"Received 0 bytes");
    return;
  }

  if (!_recv_buffer.OnWrite(num_of_bytes)) {
    Disconnect(L"OnWrite Overflow");
    return;
  }

  const int32_t data_size = _recv_buffer.GetDataSize();
  const int32_t process_len = OnRecv(_recv_buffer.GetReadPos(), data_size);

  if (process_len < 0 || data_size < process_len || !_recv_buffer.OnRead(process_len)) {
    Disconnect(L"OnRead Error");
    return;
  }

  _recv_buffer.Clear();

  RegisterRecv();
}

void Session::ProcessSend(const int32_t num_of_bytes) {
  _send_event.SetOwner(nullptr);
  _send_event.ClearBuffers();

  if (num_of_bytes == 0) {
    Disconnect(L"Send 0");
    return;
  }

  OnSend(num_of_bytes);

  WRITE_LOCK;

  if (_send_queue.empty()) {
    _send_registered.store(false);
  } else {
    RegisterSend();
  }
}

void Session::HandleError(const int32_t error_code) {
  switch (error_code) {
    case WSAECONNRESET: [[fallthrough]];
    case WSAECONNABORTED:
      Disconnect(L"HandleError");
      break;
    default:
      std::cout << "Session::HandleError: " << error_code << '\n';
      break;
  }
}

int32_t PacketSession::OnRecv(std::byte* buffer, const int32_t len) {
  int32_t process_len = 0;

  while (true) {
    int32_t data_size = len - process_len;

    if (data_size < sizeof(PacketHeader)) {
      break;
    }

    const PacketHeader header = *reinterpret_cast<PacketHeader*>(&buffer[process_len]);

    if (data_size < header.size) {
      break;
    }

    OnRecvPacket(&buffer[process_len], header.size);
    process_len += header.size;
  }

  return process_len;
}
