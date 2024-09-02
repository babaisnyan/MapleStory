#pragma once

namespace game {
  class GameObject : public std::enable_shared_from_this<GameObject> {
  public:
    enum class ObjectType : uint8_t {
      kPlayer,
      kNpc,
      kMob
    };

    enum {
      kObjectRange = 10000000
    };

  public:
    virtual ~GameObject() = default;

  protected:
    explicit GameObject(int64_t object_id);

  public:
    virtual void OnEnter() = 0;
    virtual void Update(float delta_time) = 0;
    virtual void PostUpdate() = 0;

  public:
    int64_t GetObjectId() const;
    const MsCoordinate& GetPosition() const;
    float GetX() const;
    float GetY() const;

    void UpdatePosition(float x, float y, bool flip);

  protected:
    virtual int64_t GetNextObjectId() = 0;

  protected:
    int64_t _object_id;
    MsCoordinate _position = {};
    bool _flip = false;
  };
}
