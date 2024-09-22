#include "pch.h"
#include "item_template.h"

using namespace rapidjson;

void ItemTemplate::Load(const Value& data) {
  const std::string name = data["Name"].GetString();
  const std::string desc = data["Desc"].GetString();

  _name = utils::ConvertToWide(name).value_or(L"");
  _desc = utils::ConvertToWide(desc).value_or(L"");
  _id = data["Id"].GetUint();
  _item_type = static_cast<ItemType>(data["ItemType"].GetUint());
  _sub_type = static_cast<EqpType>(data["SubType"].GetUint());
  _inc_acc = data["IncAcc"].GetUint();
  _inc_dex = data["IncDex"].GetUint();
  _inc_eva = data["IncEva"].GetUint();
  _inc_int = data["IncInt"].GetUint();
  _inc_jump = data["IncJump"].GetUint();
  _inc_luk = data["IncLuk"].GetUint();
  _inc_mad = data["IncMad"].GetUint();
  _inc_mdd = data["IncMdd"].GetUint();
  _inc_mhp = data["IncMhp"].GetUint();
  _inc_pad = data["IncPad"].GetUint();
  _inc_pdd = data["IncPdd"].GetUint();
  _inc_speed = data["IncSpeed"].GetUint();
  _price = data["Price"].GetUint();
  _slot_max = data["SlotMax"].GetUint();
  _req_level = data["ReqLevel"].GetUint();
  _req_str = data["ReqStr"].GetUint();
  _req_dex = data["ReqDex"].GetUint();
  _req_int = data["ReqInt"].GetUint();
  _req_luk = data["ReqLuk"].GetUint();
  _inc_str = data["IncStr"].GetUint();
  _inc_mmd = data["IncMmd"].GetUint();
  _hp = data["Hp"].GetInt();
  _mp = data["Mp"].GetInt();
  _time = data["Time"].GetUint();
}

const String& ItemTemplate::GetDesc() const {
  return _desc;
}

ItemType ItemTemplate::GetItemType() const {
  return _item_type;
}

EqpType ItemTemplate::GetSubType() const {
  return _sub_type;
}

uint16_t ItemTemplate::GetIncAcc() const {
  return _inc_acc;
}

uint16_t ItemTemplate::GetIncDex() const {
  return _inc_dex;
}

uint16_t ItemTemplate::GetIncEva() const {
  return _inc_eva;
}

uint16_t ItemTemplate::GetIncInt() const {
  return _inc_int;
}

uint16_t ItemTemplate::GetIncJump() const {
  return _inc_jump;
}

uint16_t ItemTemplate::GetIncLuk() const {
  return _inc_luk;
}

uint16_t ItemTemplate::GetIncMad() const {
  return _inc_mad;
}

uint16_t ItemTemplate::GetIncMdd() const {
  return _inc_mdd;
}

uint16_t ItemTemplate::GetIncMhp() const {
  return _inc_mhp;
}

uint16_t ItemTemplate::GetIncPad() const {
  return _inc_pad;
}

uint16_t ItemTemplate::GetIncPdd() const {
  return _inc_pdd;
}

uint16_t ItemTemplate::GetIncSpeed() const {
  return _inc_speed;
}

uint32_t ItemTemplate::GetPrice() const {
  return _price;
}

uint16_t ItemTemplate::GetSlotMax() const {
  return _slot_max;
}

uint16_t ItemTemplate::GetReqLevel() const {
  return _req_level;
}

uint16_t ItemTemplate::GetReqStr() const {
  return _req_str;
}

uint16_t ItemTemplate::GetReqDex() const {
  return _req_dex;
}

uint16_t ItemTemplate::GetReqInt() const {
  return _req_int;
}

uint16_t ItemTemplate::GetReqLuk() const {
  return _req_luk;
}

uint16_t ItemTemplate::GetIncStr() const {
  return _inc_str;
}

uint16_t ItemTemplate::GetIncMmd() const {
  return _inc_mmd;
}

int32_t ItemTemplate::GetHp() const {
  return _hp;
}

int32_t ItemTemplate::GetMp() const {
  return _mp;
}

uint32_t ItemTemplate::GetTime() const {
  return _time;
}
