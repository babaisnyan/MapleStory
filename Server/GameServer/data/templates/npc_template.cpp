#include "pch.h"
#include "npc_template.h"

using namespace rapidjson;

void NpcTemplate::Load(const Value& data) {
  const std::string name = data["NpcName"].GetString();

  _name = utils::ConvertToWide(name).value_or(L"");
  _id = data["Id"].GetInt();
  _is_shop = data["IsShop"].GetBool();

  for (const auto& action_data : data["Actions"].GetObj()) {
    auto time = action_data.value["Length"].GetInt();
    const auto action = utils::ConvertToWide(action_data.name.GetString());

    if (action.has_value()) {
      _actions.emplace(action.value(), time);
    }
  }
}

const std::unordered_map<String, int32_t>& NpcTemplate::GetActions() const {
  return _actions;
}
