#pragma once

namespace game {
  class MobTemplate;

  class MobProvider {
  public:
    static MobProvider& GetInstance() {
      static MobProvider instance;
      return instance;
    }

  public:
    void Init();

    std::optional<std::shared_ptr<MobTemplate>> GetItem(const uint32_t id) const {
      const auto it = _mobs.find(id);
      if (it == _mobs.end()) {
        return std::nullopt;
      }

      return it->second;
    }

  private:
    std::unordered_map<uint32_t, std::shared_ptr<MobTemplate>> _mobs;
  };
}
