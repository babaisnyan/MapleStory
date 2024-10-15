#include "pch.h"
#include "player_stat.h"

#include "item.h"

#include "inventory/inventory.h"

constexpr uint64_t kExpTable[] = {
  0, 15, 34, 57, 92, 135, 372, 560, 840, 1242,
  1242, 1242, 1242, 1242, 1242, 1490, 1788, 2145, 2574, 3088,
  3705, 4446, 5335, 6402, 7682, 9218, 11061, 13273, 15927, 19112,
  19112, 19112, 19112, 19112, 19112, 22934, 27520, 33024, 39628, 47553,
  51357, 55465, 59902, 64694, 69869, 75458, 81494, 88013, 95054, 102658,
  110870, 119739, 129318, 139663, 150836, 162902, 175934, 190008, 205208, 221624,
  221624, 221624, 221624, 221624, 221624, 238245, 256113, 275321, 295970, 318167,
  342029, 367681, 395257, 424901, 456768, 488741, 522952, 559558, 598727, 640637,
  685481, 733464, 784806, 839742, 898524, 961420, 1028719, 1100729, 1177780, 1260224,
  1342138, 1429377, 1522286, 1621234, 1726614, 1838844, 1958369, 2085663, 2221231, 2365611,
  2365611, 2365611, 2365611, 2365611, 2365611, 2519375, 2683134, 2857537, 3043277, 3241090,
  3451761, 3676125, 3915073, 4169553, 4440574, 4729211, 5036610, 5363990, 5712649, 6083971,
  6479429, 6900592, 7349131, 7826825, 8335569, 8877381, 9454411, 10068948, 10723430, 11420454,
  12162784, 12953366, 13795336, 14692034, 15647017, 16664074, 17747240, 18900812, 20129366, 21437776,
  22777636, 24201238, 25713816, 27320930, 29028488, 30842768, 32770440, 34818592, 36994752, 39306924,
  41763608, 44373832, 47147196, 50093896, 53224764, 56551312, 60085768, 63841128, 67831200, 72070648,
  76575064, 81361008, 86446072, 91848952, 97589512, 103688856, 110169408, 117054992, 124370928, 132144112,
  138751312, 145688864, 152973296, 160621952, 168653040, 177085680, 185939952, 195236944, 204998784, 215248720,
  226011152, 237311696, 249177264, 261636112, 274717920, 288453792, 302876480, 318020288, 333921280, 350617344,
  368148192, 386555584, 405883360, 426177504, 447486368, 469860672, 493353696, 518021344, 543922368, 571118464,
  2207026470, 2471869696, 2768494080, 3100713472, 3472798976, 3889534976, 4356279296, 4879032832, 5464516608, 6120258560,
  7956335616, 8831533056, 9803001856, 10881332224, 12078278656, 15701762048, 17114920960, 18655264768, 20334239744, 22164322304,
  28813617152, 30830571520, 32988712960, 35297923072, 37768777728, 49099407360, 52536369152, 56213917696, 60148895744, 64359321600,
  83667116032, 86177128448, 88762441728, 91425308672, 94168064000, 122418479104, 126091026432, 129873756160, 133769961472, 137783050240,
  179117965312, 184491491328, 190026235904, 195727015936, 201598828544, 262078464000, 269940817920, 278039035904, 286380195840, 294971604992,
  442457391104, 455731085312, 469403009024, 483485089792, 497989615616, 512929300480, 528317153280, 544166641664, 560491593728, 577306296320,
  1731918888960, 1749238022144, 1766730366976, 1784397692928, 1802241703936, 2342914097152, 2366343217152, 2390006693888, 2413906624512, 2438045630464,
  5412461477888, 5466585825280, 5521251762176, 5576464007168, 5632228851712, 11377102028800, 12514812231680, 13766293979136, 15142923272192, 16657215913984,
  33647574908928, 37012333658112, 40713567862784, 44784924229632, 49263417491456, 99512102158336, 109463314890752, 120409651412992, 132450617393152, 145695675777024,
  294305251983360, 323735777181696, 356109361610752, 391720311193600, 430892359090176, 870403132500696, 957443445750765, 1053187790325841, 1158506569358425, 1737759854037637, 0
};

void PlayerStat::UpdateStats() {
  _base_physical_defense = _level * 2.5;
  _base_magical_defense = _level * 2.5;
  _base_magical_attack = _level * 1.5;
  _base_physical_attack = _level * 1.5;

  _physical_defense = _base_physical_defense + _extra_pdd;
  _magical_defense = _base_magical_defense + _extra_mdd;
  _magical_attack = _base_magical_attack + _extra_mad;
  _physical_attack = _base_physical_attack + _extra_pad;

  _stat_base = (_str + _extra_str + _dex + _extra_dex + _int + _extra_int + _luk + _extra_luk) * 0.25;

  _is_dirty = false;
}

void PlayerStat::ApplyEquip(const std::shared_ptr<Inventory>& inventory) {
  _is_dirty = true;
  _extra_str = 0;
  _extra_dex = 0;
  _extra_int = 0;
  _extra_luk = 0;
  _extra_pad = 0;
  _extra_pdd = 0;
  _extra_mad = 0;
  _extra_mdd = 0;
  _extra_hp = 0;

  for (const auto& item : inventory->GetAllItems(Inventory::kEquipped) | std::views::values) {
    const auto item_template = item->GetItemTemplate();

    _extra_str += item_template->GetIncStr();
    _extra_dex += item_template->GetIncDex();
    _extra_int += item_template->GetIncInt();
    _extra_luk += item_template->GetIncLuk();
    _extra_pad += item_template->GetIncPad();
    _extra_pdd += item_template->GetIncPdd();
    _extra_mad += item_template->GetIncMad();
    _extra_mdd += item_template->GetIncMdd();
    _extra_hp += item_template->GetIncMhp();
  }
}

int32_t PlayerStat::GetStr() const {
  return _str;
}

void PlayerStat::SetStr(const int32_t str) {
  _is_dirty = true;
  _str = str;
}

int32_t PlayerStat::GetDex() const {
  return _dex;
}

void PlayerStat::SetDex(const int32_t dex) {
  _is_dirty = true;
  _dex = dex;
}

int32_t PlayerStat::GetInt() const {
  return _int;
}

void PlayerStat::SetInt(const int32_t int_) {
  _is_dirty = true;
  _int = int_;
}

int32_t PlayerStat::GetLuk() const {
  return _luk;
}

void PlayerStat::SetLuk(const int32_t luk) {
  _is_dirty = true;
  _luk = luk;
}

int32_t PlayerStat::GetAp() const {
  return _ap;
}

void PlayerStat::SetAp(const int32_t ap) {
  _ap = ap;
}

int32_t PlayerStat::GetSp() const {
  return _sp;
}

void PlayerStat::SetSp(const int32_t sp) {
  _sp = sp;
}

int32_t PlayerStat::GetBuffedMaxHp() const {
  return _extra_hp + _max_hp;
}

int32_t PlayerStat::GetMp() const {
  return _mp;
}

void PlayerStat::SetMp(const int32_t mp) {
  _is_dirty = true;
  _mp = mp;
}

int32_t PlayerStat::GetMaxMp() const {
  return _max_mp;
}

void PlayerStat::SetMaxMp(const int32_t max_mp) {
  _is_dirty = true;
  _max_mp = max_mp;
}

uint64_t PlayerStat::GetLastCollisionTime() const {
  return _last_collision_time;
}

void PlayerStat::SetLastCollisionTime(const uint64_t last_collision_time) {
  _last_collision_time = last_collision_time;
}

double PlayerStat::GetStatBase() const {
  return _stat_base;
}

uint64_t PlayerStat::GetMaxExp() const {
  return kExpTable[_level];
}
