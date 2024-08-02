#pragma once

namespace game {
  class GameObject : std::enable_shared_from_this<GameObject> {
  public:
    GameObject();
    virtual ~GameObject() = default;

  public:
    virtual void OnEnter() = 0;
    virtual void Update(float delta_time) = 0;

  public:
    int64_t GetObjectId() const;

  protected:
    int64_t _object_id;
  };
}
