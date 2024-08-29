#include "pch.h"
#include "map_template.h"

void MapTemplate::Load(const json& data) {
  std::string name;

  data.at("Name").get_to(name);
  _id = data["Id"].get<int>();
  _name = utils::ConvertToWide(name).value_or(L"");

  for (const auto& mob : data["Mobs"]) {
    _mobs.emplace_back(mob["Id"].get<uint32_t>(), mob["X"].get<float>(), mob["Y"].get<float>());
  }
}

uint32_t MapTemplate::GetId() const {
  return _id;
}

const String& MapTemplate::GetName() const {
  return _name;
}

const std::vector<SpawnPoint>& MapTemplate::GetMobs() const {
  return _mobs;
}
