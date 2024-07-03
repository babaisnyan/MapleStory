#include "pch.h"
#include "socket_utils.h"

void SocketUtils::Init() {
  WSADATA wsa_data;

  ASSERT_CRASH(WSAStartup(MAKEWORD(2, 2), &wsa_data) == 0);

  SOCKET socket = CreateSocket();

  ASSERT_CRASH(BindWindowsFunction(socket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)));
  ASSERT_CRASH(BindWindowsFunction(socket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)));
  ASSERT_CRASH(BindWindowsFunction(socket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)));

  Close(socket);
}

void SocketUtils::Clear() {
  WSACleanup();
}

bool SocketUtils::BindWindowsFunction(const SOCKET socket, GUID guid, LPVOID* func) {
  DWORD bytes = 0;

  const int32_t result = WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER,
                                &guid, sizeof(guid),
                                func, sizeof(*func),
                                &bytes, nullptr, nullptr);

  return result != SOCKET_ERROR;
}

SOCKET SocketUtils::CreateSocket() {
  return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP,
                   nullptr, 0,WSA_FLAG_OVERLAPPED);
}

bool SocketUtils::SetLinger(const SOCKET socket, const uint16_t flag, const uint16_t linger) {
  const LINGER option = {flag, linger};

  return SetSocketOption(socket, SOL_SOCKET, SO_LINGER, option);
}

bool SocketUtils::SetRecvBufferSize(const SOCKET socket, const int32_t size) {
  return SetSocketOption(socket, SOL_SOCKET, SO_RCVBUF, size);
}

bool SocketUtils::SetSendBufferSize(const SOCKET socket, const int32_t size) {
  return SetSocketOption(socket, SOL_SOCKET, SO_SNDBUF, size);
}

bool SocketUtils::SetReuseAddress(const SOCKET socket, const bool flag) {
  return SetSocketOption(socket, SOL_SOCKET, SO_REUSEADDR, flag);
}

bool SocketUtils::SetTcpNoDelay(const SOCKET socket, const bool flag) {
  return SetSocketOption(socket, IPPROTO_TCP, TCP_NODELAY, flag);
}

bool SocketUtils::SetUpdateAcceptSocket(const SOCKET socket, const SOCKET listen_socket) {
  return SetSocketOption(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listen_socket);
}

bool SocketUtils::Bind(const SOCKET socket, NetworkAddress address) {
  const int32_t result = bind(socket,
                            reinterpret_cast<const SOCKADDR*>(&address.GetSocketAddress()),
                            sizeof(SOCKADDR_IN));

  return result != SOCKET_ERROR;
}

bool SocketUtils::BindAnyAddress(const SOCKET socket, const uint16_t port) {
  SOCKADDR_IN address = {};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(port);

  const int32_t result = bind(socket, reinterpret_cast<const SOCKADDR*>(&address),
                            sizeof(address));

  return result != SOCKET_ERROR;
}

bool SocketUtils::Listen(const SOCKET socket, const int32_t backlog) {
  return listen(socket, backlog) != SOCKET_ERROR;
}

void SocketUtils::Close(SOCKET& socket) {
  if (socket != INVALID_SOCKET) {
    closesocket(socket);
  }

  socket = INVALID_SOCKET;
}
