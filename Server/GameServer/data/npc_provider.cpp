#include "pch.h"
#include "npc_provider.h"

#include "templates/npc_template.h"

#include <fstream>

void NpcProvider::Init() {
  std::ifstream file("Data/Npc.json");
  const auto json = json::parse(file);

  for (const auto& npc : json) {
    auto npc_template = std::make_shared<NpcTemplate>();
    npc_template->Load(npc);
    _npcs.emplace(npc_template->GetId(), npc_template);
  }
}
