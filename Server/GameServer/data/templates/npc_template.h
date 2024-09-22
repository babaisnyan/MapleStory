#pragma once

#include "template.h"

namespace game {
  class NpcTemplate : public Template {
  public:
    NpcTemplate() = default;

    void Load(const rapidjson::Value& data) override;

  public:
    const std::unordered_map<String, int32_t>& GetActions() const;

  private:
    bool _is_shop;
    std::unordered_map<String, int32_t> _actions;
  };
}
