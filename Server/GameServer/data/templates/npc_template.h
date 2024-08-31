#pragma once

namespace game {
  class NpcTemplate {
  public:
    NpcTemplate() = default;

    void Load(const json& data);

  public:
    uint32_t GetId() const;
    const String& GetName() const;
    int32_t GetAction(const String& action) const;
    const std::unordered_map<String, int32_t>& GetActions() const;

  private:
    uint32_t _id;
    bool _is_shop;
    String _name;
    std::unordered_map<String, int32_t> _actions;
  };
}
