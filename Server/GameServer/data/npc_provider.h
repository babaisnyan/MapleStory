#pragma once

namespace game {
  class NpcTemplate;

  class NpcProvider {
  public:
    static NpcProvider& GetInstance() {
      static NpcProvider instance;
      return instance;
    }

  public:
    void Init();

    std::optional<std::shared_ptr<NpcTemplate>> GetNpc(const uint32_t id) const {
      const auto it = _npcs.find(id);
      if (it == _npcs.end()) {
        return std::nullopt;
      }

      return it->second;
    }

  private:
    std::unordered_map<uint32_t, std::shared_ptr<NpcTemplate>> _npcs;
  };
}
