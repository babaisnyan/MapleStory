#pragma once

class NetworkAddress {
public:
  NetworkAddress() = default;
  explicit NetworkAddress(SOCKADDR_IN socket_address);
  NetworkAddress(const std::wstring& ip, uint16_t port);

  SOCKADDR_IN& GetSocketAddress() noexcept {
    return _socket_address;
  }

  std::wstring GetIpAddress() const;

  uint16_t GetPort() const {
    return ntohs(_socket_address.sin_port);
  }

public:
  static IN_ADDR IpToAddress(const WCHAR* ip);

private:
  SOCKADDR_IN _socket_address = {};
};
