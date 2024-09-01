#include "pch.h"
#include "map_template.h"

void MapTemplate::Load(const json& data) {
  std::string name;

  data.at("Name").get_to(name);
  _id = data["Id"].get<int32_t>();
  _name = utils::ConvertToWide(name).value_or(L"");
  _size = {data["Width"].get<int32_t>(), data["Height"].get<int32_t>()};

  for (const auto& ground : data["Grounds"]) {
    _grounds.emplace_back(ground["Id"].get<int32_t>(),
                          ground["StartX"].get<int32_t>(),
                          ground["StartY"].get<int32_t>(),
                          ground["EndX"].get<int32_t>(),
                          ground["EndY"].get<int32_t>());
  }

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

std::pair<int32_t, int32_t> MapTemplate::GetSize() const {
  return _size;
}

const std::vector<SpawnPoint>& MapTemplate::GetMobs() const {
  return _mobs;
}

std::vector<GroundInfo> MapTemplate::GetGrounds() const {
  return _grounds;
}
