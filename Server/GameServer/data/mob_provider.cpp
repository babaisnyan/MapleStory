#include "pch.h"
#include "mob_provider.h"

#include <fstream>

#include "templates/mob_template.h"

void MobProvider::Init() {
  std::ifstream file("Data/Mobs.json");
  const auto mobs = json::parse(file);

  for (const auto& mob : mobs) {
    auto mob_template = std::make_shared<MobTemplate>();
    mob_template->Load(mob);
    _mobs.emplace(mob_template->GetId(), mob_template);
  }
}
