#pragma once

namespace game {
  class MapTemplate;

  class MapProvider {
  public:
    static MapProvider& GetInstance() {
      static MapProvider instance;
      return instance;
    }

  public:
    void Init();

    std::optional<std::shared_ptr<MapTemplate>> GetItem(const uint32_t id) const {
      const auto it = _maps.find(id);
      if (it == _maps.end()) {
        return std::nullopt;
      }

      return it->second;
    }

  private:
    std::unordered_map<uint32_t, std::shared_ptr<MapTemplate>> _maps;
  };
}
