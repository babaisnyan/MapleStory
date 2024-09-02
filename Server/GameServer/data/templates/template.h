#pragma once

namespace game {
  class Template {
  protected:
    Template() = default;

  public:
    virtual ~Template() = default;
    virtual void Load(const json& data) = 0;

    Template(const Template&) = delete;
    Template& operator=(Template&) = delete;

  public:
    uint32_t GetId() const;
    String GetName() const;

  protected:
    uint32_t _id = 0;
    String _name;
  };
}
