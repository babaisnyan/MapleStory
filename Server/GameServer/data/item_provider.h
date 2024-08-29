#pragma once

namespace game {
  class ItemTemplate;

  class ItemProvider {
  public:
    static ItemProvider& GetInstance() {
      static ItemProvider instance;
      return instance;
    }

  public:
    void Init();

    std::optional<std::shared_ptr<ItemTemplate>> GetItem(const uint32_t id) const {
      const auto it = _items.find(id);
      if (it == _items.end()) {
        return std::nullopt;
      }

      return it->second;
    }

  private:
    std::unordered_map<uint32_t, std::shared_ptr<ItemTemplate>> _items;
  };
}
