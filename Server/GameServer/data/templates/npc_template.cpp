#include "pch.h"
#include "npc_template.h"

void NpcTemplate::Load(const json& data) {
  std::string name;

  data.at("Id").get_to(_id);
  data.at("NpcName").get_to(name);
  data.at("IsShop").get_to(_is_shop);
  _name = utils::ConvertToWide(name).value_or(L"");

  for (const auto& [key, value] : data.at("Actions").items()) {
    auto time = 0;
    const auto action = utils::ConvertToWide(key);
    value.at("Length").get_to(time);

    if (action.has_value()) {
      _actions.emplace(action.value(), time);
    }
  }
}

const std::unordered_map<String, int32_t>& NpcTemplate::GetActions() const {
  return _actions;
}
