#include "pch.h"
#include "server_config.h"

#include <fstream>

void ServerConfig::Init(const String& file_name) {
  const std::filesystem::path path(file_name);
  std::ifstream file(L"Config" / path);
  auto data = json::parse(file);
  std::string name;
  std::string ip;
  std::string center_ip;

  data.at("ServerName").get_to(name);
  data.at("ServerIP").get_to(ip);
  data.at("CenterIP").get_to(center_ip);
  _server_name = utils::ConvertToWide(name).value();
  _server_ip = utils::ConvertToWide(ip).value();
  _center_ip = utils::ConvertToWide(center_ip).value();
  data.at("ServerPort").get_to(_server_port);
  data.at("CenterPort").get_to(_center_port);
}

const String& ServerConfig::GetServerName() const { return _server_name; }

const String& ServerConfig::GetServerIp() const { return _server_ip; }

const String& ServerConfig::GetCenterIp() const { return _center_ip; }

uint16_t ServerConfig::GetServerPort() const { return _server_port; }

uint16_t ServerConfig::GetCenterPort() const { return _center_port; }
