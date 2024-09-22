#include "pch.h"
#include "npc_provider.h"

#include <fstream>

#include "templates/npc_template.h"

using namespace rapidjson;

void NpcProvider::Init() {
  std::ifstream file("Data/Npc.json");
  IStreamWrapper stream(file);

  Document data;
  data.ParseStream(stream);
  assert(data.IsArray());

  for (const auto& npc : data.GetArray()) {
    auto npc_template = std::make_shared<NpcTemplate>();
    npc_template->Load(npc);
    _npcs.emplace(npc_template->GetId(), npc_template);
  }
}
