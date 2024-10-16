﻿#pragma once

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

  public:
    int64_t GetObjectId() const;
    ObjectType GetObjectType() const;
    const MsCoordinate& GetPosition() const;
    float GetX() const;
    float GetY() const;
    bool IsFlipped() const;
    void SetFlip(bool flip);
    void SetBounds(const std::tuple<int32_t, int32_t, int32_t, int32_t>& bounds);
    void UpdatePosition(float x, float y, bool flip);
    void SetAlive(bool is_alive);
    bool IsAlive() const;

  protected:
    virtual int64_t GetNextObjectId() = 0;

  protected:
    int64_t _object_id;
    MsCoordinate _position = {};
    bool _flip = false; // 기본값은 오른쪽임
    bool _is_alive = true;
  };
}
