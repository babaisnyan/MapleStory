#include "pch.h"
#include "network_address.h"

NetworkAddress::NetworkAddress(const SOCKADDR_IN socket_address) :
  _socket_address(socket_address) {}

NetworkAddress::NetworkAddress(const std::wstring& ip, const uint16_t port) {
  _socket_address.sin_family = AF_INET;
  _socket_address.sin_addr = IpToAddress(ip.c_str());
  _socket_address.sin_port = htons(port);
}

std::wstring NetworkAddress::GetIpAddress() const {
  WCHAR buffer[100];
  InetNtopW(AF_INET, &_socket_address.sin_addr, buffer, length32(buffer));
  return {buffer};
}

IN_ADDR NetworkAddress::IpToAddress(const WCHAR* ip) {
  IN_ADDR address;

  InetPtonW(AF_INET, ip, &address);

  return address;
}
