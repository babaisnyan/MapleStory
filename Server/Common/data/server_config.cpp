#include "pch.h"
#include "server_config.h"

#include <filesystem>
#include <fstream>

using namespace rapidjson;

constexpr std::array kConfigKeys = {
  "ServerName", "ServerIP", "ServerPort", "CenterIP", "CenterPort"
};

void ServerConfig::Init(const String& file_name) {
  const std::filesystem::path path(file_name);
  std::ifstream file(L"Config" / path);
  IStreamWrapper stream(file);

  Document data;
  data.ParseStream(stream);
  assert(data.IsObject());

  for (const auto& key : kConfigKeys) {
    assert(data.HasMember(key));
  }

  std::string name = data["ServerName"].GetString();
  std::string ip = data["ServerIP"].GetString();
  std::string center_ip = data["CenterIP"].GetString();

  _server_port = static_cast<uint16_t>(data["ServerPort"].GetUint());
  _center_port = static_cast<uint16_t>(data["CenterPort"].GetUint());
  _server_name = utils::ConvertToWide(name).value();
  _server_ip = utils::ConvertToWide(ip).value();
  _center_ip = utils::ConvertToWide(center_ip).value();
}

const String& ServerConfig::GetServerName() const {
  return _server_name;
}

const String& ServerConfig::GetServerIp() const {
  return _server_ip;
}

const String& ServerConfig::GetCenterIp() const {
  return _center_ip;
}

uint16_t ServerConfig::GetServerPort() const {
  return _server_port;
}

uint16_t ServerConfig::GetCenterPort() const {
  return _center_port;
}
