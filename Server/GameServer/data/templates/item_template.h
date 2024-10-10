#pragma once

#include "template.h"

namespace game {
  enum class ItemType : uint8_t {
    kEquip,
    kUse,
    kEtc
  };

  enum class EqpType : uint8_t {
    kAccessory,
    kCap,
    kCape,
    kCoat,
    kGlove,
    kLongcoat,
    kPants,
    kRing,
    kShield,
    kShoes,
    kWeapon
  };

  class ItemTemplate : public Template {
  public:
    ItemTemplate() = default;

    void Load(const rapidjson::Value& data) override;

  public:
    const String& GetDesc() const;
    ItemType GetItemType() const;
    EqpType GetSubType() const;
    uint16_t GetIncAcc() const;
    uint16_t GetIncDex() const;
    uint16_t GetIncEva() const;
    uint16_t GetIncInt() const;
    uint16_t GetIncJump() const;
    uint16_t GetIncLuk() const;
    uint16_t GetIncMad() const;
    uint16_t GetIncMdd() const;
    uint16_t GetIncMhp() const;
    uint16_t GetIncPad() const;
    uint16_t GetIncPdd() const;
    uint16_t GetIncSpeed() const;
    uint32_t GetPrice() const;
    uint16_t GetSlotMax() const;
    uint16_t GetReqLevel() const;
    uint16_t GetReqStr() const;
    uint16_t GetReqDex() const;
    uint16_t GetReqInt() const;
    uint16_t GetReqLuk() const;
    uint16_t GetIncStr() const;
    uint16_t GetIncMmd() const;
    int32_t GetHp() const;
    int32_t GetMp() const;
    uint32_t GetTime() const;

  private:
    String _desc;
    ItemType _item_type;
    EqpType _sub_type;
    uint16_t _inc_acc;
    uint16_t _inc_dex;
    uint16_t _inc_eva;
    uint16_t _inc_int;
    uint16_t _inc_jump;
    uint16_t _inc_luk;
    uint16_t _inc_mad;
    uint16_t _inc_mdd;
    uint16_t _inc_mhp;
    uint16_t _inc_pad;
    uint16_t _inc_pdd;
    uint16_t _inc_speed;
    uint32_t _price;
    uint16_t _slot_max;
    uint16_t _req_level;
    uint16_t _req_str;
    uint16_t _req_dex;
    uint16_t _req_int;
    uint16_t _req_luk;
    uint16_t _inc_str;
    uint16_t _inc_mmd;
    int32_t _hp;
    int32_t _mp;
    uint32_t _time;
  };
}
