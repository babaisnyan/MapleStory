#include "pch.h"
#include "item_template.h"

void ItemTemplate::Load(const json& data) {
  std::string name;
  std::string desc;

  data.at("Name").get_to(name);
  data.at("Desc").get_to(desc);
  _name = utils::ConvertToWide(name).value_or(L"");
  _desc = utils::ConvertToWide(desc).value_or(L"");
  data.at("Id").get_to(_id);
  data.at("ItemType").get_to(_item_type);
  data.at("SubType").get_to(_sub_type);
  data.at("IncAcc").get_to(_inc_acc);
  data.at("IncDex").get_to(_inc_dex);
  data.at("IncEva").get_to(_inc_eva);
  data.at("IncInt").get_to(_inc_int);
  data.at("IncJump").get_to(_inc_jump);
  data.at("IncLuk").get_to(_inc_luk);
  data.at("IncMad").get_to(_inc_mad);
  data.at("IncMdd").get_to(_inc_mdd);
  data.at("IncMhp").get_to(_inc_mhp);
  data.at("IncPad").get_to(_inc_pad);
  data.at("IncPdd").get_to(_inc_pdd);
  data.at("IncSpeed").get_to(_inc_speed);
  data.at("Price").get_to(_price);
  data.at("SlotMax").get_to(_slot_max);
  data.at("ReqLevel").get_to(_req_level);
  data.at("ReqStr").get_to(_req_str);
  data.at("ReqDex").get_to(_req_dex);
  data.at("ReqInt").get_to(_req_int);
  data.at("ReqLuk").get_to(_req_luk);
  data.at("IncStr").get_to(_inc_str);
  data.at("IncMmd").get_to(_inc_mmd);
  data.at("Hp").get_to(_hp);
  data.at("Mp").get_to(_mp);
  data.at("Time").get_to(_time);
}

uint32_t ItemTemplate::GetId() const { return _id; }

const String& ItemTemplate::GetName() const { return _name; }

const String& ItemTemplate::GetDesc() const { return _desc; }

ItemType ItemTemplate::GetItemType() const { return _item_type; }

EqpType ItemTemplate::GetSubType() const { return _sub_type; }

uint16_t ItemTemplate::GetIncAcc() const { return _inc_acc; }

uint16_t ItemTemplate::GetIncDex() const { return _inc_dex; }

uint16_t ItemTemplate::GetIncEva() const { return _inc_eva; }

uint16_t ItemTemplate::GetIncInt() const { return _inc_int; }

uint16_t ItemTemplate::GetIncJump() const { return _inc_jump; }

uint16_t ItemTemplate::GetIncLuk() const { return _inc_luk; }

uint16_t ItemTemplate::GetIncMad() const { return _inc_mad; }

uint16_t ItemTemplate::GetIncMdd() const { return _inc_mdd; }

uint16_t ItemTemplate::GetIncMhp() const { return _inc_mhp; }

uint16_t ItemTemplate::GetIncPad() const { return _inc_pad; }

uint16_t ItemTemplate::GetIncPdd() const { return _inc_pdd; }

uint16_t ItemTemplate::GetIncSpeed() const { return _inc_speed; }

uint32_t ItemTemplate::GetPrice() const { return _price; }

uint16_t ItemTemplate::GetSlotMax() const { return _slot_max; }

uint16_t ItemTemplate::GetReqLevel() const { return _req_level; }

uint16_t ItemTemplate::GetReqStr() const { return _req_str; }

uint16_t ItemTemplate::GetReqDex() const { return _req_dex; }

uint16_t ItemTemplate::GetReqInt() const { return _req_int; }

uint16_t ItemTemplate::GetReqLuk() const { return _req_luk; }

uint16_t ItemTemplate::GetIncStr() const { return _inc_str; }

uint16_t ItemTemplate::GetIncMmd() const { return _inc_mmd; }

uint32_t ItemTemplate::GetHp() const { return _hp; }

uint32_t ItemTemplate::GetMp() const { return _mp; }

uint32_t ItemTemplate::GetTime() const { return _time; }
