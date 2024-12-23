// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: game_enum.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_game_5fenum_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_game_5fenum_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3017000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3017001 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_game_5fenum_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_game_5fenum_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_game_5fenum_2eproto;
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE
namespace protocol {

enum PlayerAnimation : int {
  PLAYER_ANIMATION_UNSPECIFIED = 0,
  PLAYER_ANIMATION_IDLE = 1,
  PLAYER_ANIMATION_RUN = 2,
  PLAYER_ANIMATION_JUMP = 3,
  PLAYER_ANIMATION_ATTACK = 4,
  PlayerAnimation_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  PlayerAnimation_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool PlayerAnimation_IsValid(int value);
constexpr PlayerAnimation PlayerAnimation_MIN = PLAYER_ANIMATION_UNSPECIFIED;
constexpr PlayerAnimation PlayerAnimation_MAX = PLAYER_ANIMATION_ATTACK;
constexpr int PlayerAnimation_ARRAYSIZE = PlayerAnimation_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* PlayerAnimation_descriptor();
template<typename T>
inline const std::string& PlayerAnimation_Name(T enum_t_value) {
  static_assert(::std::is_same<T, PlayerAnimation>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function PlayerAnimation_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    PlayerAnimation_descriptor(), enum_t_value);
}
inline bool PlayerAnimation_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, PlayerAnimation* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<PlayerAnimation>(
    PlayerAnimation_descriptor(), name, value);
}
enum KeyCode : int {
  KEY_CODE_UNSPECIFIED = 0,
  KEY_CODE_ESC = 1,
  KEY_CODE_ONE = 2,
  KEY_CODE_TWO = 3,
  KEY_CODE_THREE = 4,
  KEY_CODE_FOUR = 5,
  KEY_CODE_FIVE = 6,
  KEY_CODE_SIX = 7,
  KEY_CODE_SEVEN = 8,
  KEY_CODE_EIGHT = 9,
  KEY_CODE_NINE = 10,
  KEY_CODE_ZERO = 11,
  KEY_CODE_MINUS = 12,
  KEY_CODE_EQUAL = 13,
  KEY_CODE_Q = 16,
  KEY_CODE_W = 17,
  KEY_CODE_E = 18,
  KEY_CODE_R = 19,
  KEY_CODE_T = 20,
  KEY_CODE_Y = 21,
  KEY_CODE_U = 22,
  KEY_CODE_I = 23,
  KEY_CODE_O = 24,
  KEY_CODE_P = 25,
  KEY_CODE_OPEN_BRACKET = 26,
  KEY_CODE_CLOSE_BRACKET = 27,
  KEY_CODE_CTRL = 29,
  KEY_CODE_A = 30,
  KEY_CODE_S = 31,
  KEY_CODE_D = 32,
  KEY_CODE_F = 33,
  KEY_CODE_G = 34,
  KEY_CODE_H = 35,
  KEY_CODE_J = 36,
  KEY_CODE_K = 37,
  KEY_CODE_L = 38,
  KEY_CODE_SEMICOLON = 39,
  KEY_CODE_QUOTE = 40,
  KEY_CODE_TILDE = 41,
  KEY_CODE_SHIFT = 42,
  KEY_CODE_BACK_SLASH = 43,
  KEY_CODE_Z = 44,
  KEY_CODE_X = 45,
  KEY_CODE_C = 46,
  KEY_CODE_V = 47,
  KEY_CODE_B = 48,
  KEY_CODE_N = 49,
  KEY_CODE_M = 50,
  KEY_CODE_COMMA = 51,
  KEY_CODE_PERIOD = 52,
  KEY_CODE_ALT = 56,
  KEY_CODE_SPACE = 57,
  KEY_CODE_F1 = 59,
  KEY_CODE_F2 = 60,
  KEY_CODE_F3 = 61,
  KEY_CODE_F4 = 62,
  KEY_CODE_F5 = 63,
  KEY_CODE_F6 = 64,
  KEY_CODE_F7 = 65,
  KEY_CODE_F8 = 66,
  KEY_CODE_F9 = 67,
  KEY_CODE_F10 = 68,
  KEY_CODE_SCROLL_LOCK = 70,
  KEY_CODE_HOME = 71,
  KEY_CODE_PAGE_UP = 73,
  KEY_CODE_END = 79,
  KEY_CODE_PAGE_DOWN = 81,
  KEY_CODE_INSERT = 82,
  KEY_CODE_DELETE = 83,
  KEY_CODE_F11 = 87,
  KEY_CODE_F12 = 88,
  KeyCode_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  KeyCode_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool KeyCode_IsValid(int value);
constexpr KeyCode KeyCode_MIN = KEY_CODE_UNSPECIFIED;
constexpr KeyCode KeyCode_MAX = KEY_CODE_F12;
constexpr int KeyCode_ARRAYSIZE = KeyCode_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* KeyCode_descriptor();
template<typename T>
inline const std::string& KeyCode_Name(T enum_t_value) {
  static_assert(::std::is_same<T, KeyCode>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function KeyCode_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    KeyCode_descriptor(), enum_t_value);
}
inline bool KeyCode_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, KeyCode* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<KeyCode>(
    KeyCode_descriptor(), name, value);
}
enum KeyType : int {
  KEY_TYPE_UNSPECIFIED = 0,
  KEY_TYPE_ITEM = 1,
  KEY_TYPE_SKILL = 2,
  KEY_TYPE_CHAR_INFO = 3,
  KEY_TYPE_EQUIPMENT = 4,
  KEY_TYPE_INVENTORY = 5,
  KEY_TYPE_SKILL_WINDOW = 6,
  KEY_TYPE_ATTACK = 7,
  KEY_TYPE_JUMP = 8,
  KEY_TYPE_KEY_CONFIG = 9,
  KEY_TYPE_MENU = 10,
  KEY_TYPE_NPC_TALK = 11,
  KEY_TYPE_PICK_UP = 12,
  KeyType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  KeyType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool KeyType_IsValid(int value);
constexpr KeyType KeyType_MIN = KEY_TYPE_UNSPECIFIED;
constexpr KeyType KeyType_MAX = KEY_TYPE_PICK_UP;
constexpr int KeyType_ARRAYSIZE = KeyType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* KeyType_descriptor();
template<typename T>
inline const std::string& KeyType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, KeyType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function KeyType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    KeyType_descriptor(), enum_t_value);
}
inline bool KeyType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, KeyType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<KeyType>(
    KeyType_descriptor(), name, value);
}
enum MobActionType : int {
  MOB_ACTION_TYPE_UNSPECIFIED = 0,
  MOB_ACTION_TYPE_STAND = 1,
  MOB_ACTION_TYPE_MOVE = 2,
  MOB_ACTION_TYPE_HIT = 3,
  MOB_ACTION_TYPE_ATTACK = 4,
  MOB_ACTION_TYPE_DIE = 5,
  MOB_ACTION_TYPE_REGEN = 6,
  MobActionType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  MobActionType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool MobActionType_IsValid(int value);
constexpr MobActionType MobActionType_MIN = MOB_ACTION_TYPE_UNSPECIFIED;
constexpr MobActionType MobActionType_MAX = MOB_ACTION_TYPE_REGEN;
constexpr int MobActionType_ARRAYSIZE = MobActionType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* MobActionType_descriptor();
template<typename T>
inline const std::string& MobActionType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, MobActionType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function MobActionType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    MobActionType_descriptor(), enum_t_value);
}
inline bool MobActionType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, MobActionType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<MobActionType>(
    MobActionType_descriptor(), name, value);
}
enum ChatType : int {
  CHAT_TYPE_UNSPECIFIED = 0,
  CHAT_TYPE_NORMAL = 1,
  CHAT_TYPE_SYSTEM = 2,
  ChatType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  ChatType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool ChatType_IsValid(int value);
constexpr ChatType ChatType_MIN = CHAT_TYPE_UNSPECIFIED;
constexpr ChatType ChatType_MAX = CHAT_TYPE_SYSTEM;
constexpr int ChatType_ARRAYSIZE = ChatType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ChatType_descriptor();
template<typename T>
inline const std::string& ChatType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, ChatType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function ChatType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    ChatType_descriptor(), enum_t_value);
}
inline bool ChatType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, ChatType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<ChatType>(
    ChatType_descriptor(), name, value);
}
enum EquipSlotType : int {
  EQUIP_SLOT_TYPE_UNSPECIFIED = 0,
  EQUIP_SLOT_TYPE_CAP = 1,
  EQUIP_SLOT_TYPE_FACE = 2,
  EQUIP_SLOT_TYPE_RING = 3,
  EQUIP_SLOT_TYPE_EYE = 4,
  EQUIP_SLOT_TYPE_EAR = 5,
  EQUIP_SLOT_TYPE_SHOULDER = 6,
  EQUIP_SLOT_TYPE_CLOTHES = 7,
  EQUIP_SLOT_TYPE_PENDANT = 8,
  EQUIP_SLOT_TYPE_WEAPON = 9,
  EQUIP_SLOT_TYPE_GLOVE = 10,
  EQUIP_SLOT_TYPE_PANTS = 11,
  EQUIP_SLOT_TYPE_BELT = 12,
  EQUIP_SLOT_TYPE_SHOES = 13,
  EquipSlotType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  EquipSlotType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool EquipSlotType_IsValid(int value);
constexpr EquipSlotType EquipSlotType_MIN = EQUIP_SLOT_TYPE_UNSPECIFIED;
constexpr EquipSlotType EquipSlotType_MAX = EQUIP_SLOT_TYPE_SHOES;
constexpr int EquipSlotType_ARRAYSIZE = EquipSlotType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* EquipSlotType_descriptor();
template<typename T>
inline const std::string& EquipSlotType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, EquipSlotType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function EquipSlotType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    EquipSlotType_descriptor(), enum_t_value);
}
inline bool EquipSlotType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, EquipSlotType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<EquipSlotType>(
    EquipSlotType_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace protocol

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::protocol::PlayerAnimation> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::protocol::PlayerAnimation>() {
  return ::protocol::PlayerAnimation_descriptor();
}
template <> struct is_proto_enum< ::protocol::KeyCode> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::protocol::KeyCode>() {
  return ::protocol::KeyCode_descriptor();
}
template <> struct is_proto_enum< ::protocol::KeyType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::protocol::KeyType>() {
  return ::protocol::KeyType_descriptor();
}
template <> struct is_proto_enum< ::protocol::MobActionType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::protocol::MobActionType>() {
  return ::protocol::MobActionType_descriptor();
}
template <> struct is_proto_enum< ::protocol::ChatType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::protocol::ChatType>() {
  return ::protocol::ChatType_descriptor();
}
template <> struct is_proto_enum< ::protocol::EquipSlotType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::protocol::EquipSlotType>() {
  return ::protocol::EquipSlotType_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_game_5fenum_2eproto
