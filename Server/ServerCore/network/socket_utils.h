#pragma once
#include "network_address.h"

class SocketUtils {
public:
  static void Init();
  static void Clear();

  static bool BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* func);
  static SOCKET CreateSocket();

  static bool SetLinger(SOCKET socket, uint16_t flag, uint16_t linger);
  static bool SetRecvBufferSize(SOCKET socket, int32_t size);
  static bool SetSendBufferSize(SOCKET socket, int32_t size);
  static bool SetReuseAddress(SOCKET socket, bool flag);
  static bool SetTcpNoDelay(SOCKET socket, bool flag);
  static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET listen_socket);

  static bool Bind(SOCKET socket, NetworkAddress address);
  static bool BindAnyAddress(SOCKET socket, uint16_t port);
  static bool Listen(SOCKET socket, int32_t backlog = SOMAXCONN);
  static void Close(SOCKET& socket);

public:
  // ReSharper disable CppInconsistentNaming
  inline static LPFN_CONNECTEX ConnectEx = nullptr;
  inline static LPFN_DISCONNECTEX DisconnectEx = nullptr;
  inline static LPFN_ACCEPTEX AcceptEx = nullptr;
  // ReSharper restore CppInconsistentNaming
};

template <typename T>
static bool SetSocketOption(const SOCKET socket, const int32_t level, const int32_t name, T value) {
  const int32_t result = setsockopt(socket, level, name, reinterpret_cast<char*>(&value), sizeof(T));

  return result != SOCKET_ERROR;
}
