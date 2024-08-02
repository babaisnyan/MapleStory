#pragma once

namespace game {
  class GameObject;

  class MapInstance final : public std::enable_shared_from_this<MapInstance> {
  public:
    explicit MapInstance(const int32_t map_id);
    ~MapInstance() = default;

  public:
    void OnEnter(std::shared_ptr<GameObject> object);

  private:
    const int32_t _map_id;
  };
}
