#include "pch.h"
#include "map_template.h"

void MapTemplate::Load(const json& data) {
  std::string name;

  data.at("Name").get_to(name);
  _id = data["id"].get<int>();
  _name = utils::ConvertToWide(name).value_or(L"");

  for (const auto& mob : data["Mobs"]) {
    MobSpawnInfo info;
    info.id = mob["Id"].get<uint32_t>();
    info.x = mob["X"].get<float>();
    info.y = mob["Y"].get<float>();
    _mobs.push_back(info);
  }
}

uint32_t MapTemplate::GetId() const {
  return _id;
}

const String& MapTemplate::GetName() const {
  return _name;
}

const std::vector<MobSpawnInfo>& MapTemplate::GetMobs() const {
  return _mobs;
}
