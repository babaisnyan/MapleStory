#pragma once

class ServerConfig {
public:
  static ServerConfig& GetInstance() {
    static ServerConfig instance;
    return instance;
  }

  void Init(const String& file_name);

public:
  const String& GetServerName() const;
  const String& GetServerIp() const;
  const String& GetCenterIp() const;
  uint16_t GetServerPort() const;
  uint16_t GetCenterPort() const;

private:
  String _server_name;
  String _server_ip;
  String _center_ip;
  uint16_t _server_port;
  uint16_t _center_port;
};
