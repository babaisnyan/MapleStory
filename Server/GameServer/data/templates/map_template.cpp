#include "pch.h"
#include "map_template.h"

using namespace rapidjson;

void MapTemplate::Load(const Value& data) {
  const std::string name = data["Name"].GetString();

  _name = utils::ConvertToWide(name).value_or(L"");
  _id = data["Id"].GetInt();
  _size = {data["Width"].GetInt(), data["Height"].GetInt()};
  _bounds = {data["L"].GetInt(), data["R"].GetInt(), data["B"].GetInt(), data["T"].GetInt()};

  for (const auto& ground : data["Grounds"].GetArray()) {
    _grounds.emplace_back(ground["Id"].GetInt(),
                          ground["StartX"].GetInt(),
                          ground["StartY"].GetInt(),
                          ground["EndX"].GetInt(),
                          ground["EndY"].GetInt());
  }

  for (const auto& mob : data["Mobs"].GetArray()) {
    const int32_t ground_id = mob["Ground"].GetInt();
    const auto& ground = _grounds[ground_id];

    _mobs.emplace_back(mob["Id"].GetUint(),
                       mob["X"].GetFloat(),
                       mob["Y"].GetFloat(),
                       ground.start_x,
                       ground.end_x);
  }
}

std::pair<int32_t, int32_t> MapTemplate::GetSize() const {
  return _size;
}

std::tuple<int32_t, int32_t, int32_t, int32_t> MapTemplate::GetBounds() const {
  return _bounds;
}

const std::vector<SpawnPoint>& MapTemplate::GetMobs() const {
  return _mobs;
}

std::vector<GroundInfo> MapTemplate::GetGrounds() const {
  return _grounds;
}
