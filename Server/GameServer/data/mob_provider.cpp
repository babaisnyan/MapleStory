#include "pch.h"
#include "mob_provider.h"

#include <fstream>

#include "templates/mob_template.h"

using namespace rapidjson;

void MobProvider::Init() {
  std::ifstream file("Data/Mobs.json");
  IStreamWrapper stream(file);

  Document data;
  data.ParseStream(stream);
  assert(data.IsArray());

  for (const auto& mob : data.GetArray()) {
    auto mob_template = std::make_shared<MobTemplate>();
    mob_template->Load(mob);
    _mobs.emplace(mob_template->GetId(), mob_template);
  }
}
