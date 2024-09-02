// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: game_enum.proto

#include "game_enum.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
namespace protocol {
}  // namespace protocol
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_game_5fenum_2eproto[4];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_game_5fenum_2eproto = nullptr;
const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_game_5fenum_2eproto::offsets[1] = {};
static constexpr ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema* schemas = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_game_5fenum_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\017game_enum.proto\022\010protocol*\203\001\n\017PlayerAn"
  "imation\022 \n\034PLAYER_ANIMATION_UNSPECIFIED\020"
  "\000\022\031\n\025PLAYER_ANIMATION_IDLE\020\001\022\030\n\024PLAYER_A"
  "NIMATION_RUN\020\002\022\031\n\025PLAYER_ANIMATION_JUMP\020"
  "\003*\247\n\n\007KeyCode\022\030\n\024KEY_CODE_UNSPECIFIED\020\000\022"
  "\020\n\014KEY_CODE_ESC\020\001\022\020\n\014KEY_CODE_ONE\020\002\022\020\n\014K"
  "EY_CODE_TWO\020\003\022\022\n\016KEY_CODE_THREE\020\004\022\021\n\rKEY"
  "_CODE_FOUR\020\005\022\021\n\rKEY_CODE_FIVE\020\006\022\020\n\014KEY_C"
  "ODE_SIX\020\007\022\022\n\016KEY_CODE_SEVEN\020\010\022\022\n\016KEY_COD"
  "E_EIGHT\020\t\022\021\n\rKEY_CODE_NINE\020\n\022\021\n\rKEY_CODE"
  "_ZERO\020\013\022\022\n\016KEY_CODE_MINUS\020\014\022\022\n\016KEY_CODE_"
  "EQUAL\020\r\022\016\n\nKEY_CODE_Q\020\020\022\016\n\nKEY_CODE_W\020\021\022"
  "\016\n\nKEY_CODE_E\020\022\022\016\n\nKEY_CODE_R\020\023\022\016\n\nKEY_C"
  "ODE_T\020\024\022\016\n\nKEY_CODE_Y\020\025\022\016\n\nKEY_CODE_U\020\026\022"
  "\016\n\nKEY_CODE_I\020\027\022\016\n\nKEY_CODE_O\020\030\022\016\n\nKEY_C"
  "ODE_P\020\031\022\031\n\025KEY_CODE_OPEN_BRACKET\020\032\022\032\n\026KE"
  "Y_CODE_CLOSE_BRACKET\020\033\022\021\n\rKEY_CODE_CTRL\020"
  "\035\022\016\n\nKEY_CODE_A\020\036\022\016\n\nKEY_CODE_S\020\037\022\016\n\nKEY"
  "_CODE_D\020 \022\016\n\nKEY_CODE_F\020!\022\016\n\nKEY_CODE_G\020"
  "\"\022\016\n\nKEY_CODE_H\020#\022\016\n\nKEY_CODE_J\020$\022\016\n\nKEY"
  "_CODE_K\020%\022\016\n\nKEY_CODE_L\020&\022\026\n\022KEY_CODE_SE"
  "MICOLON\020\'\022\022\n\016KEY_CODE_QUOTE\020(\022\022\n\016KEY_COD"
  "E_TILDE\020)\022\022\n\016KEY_CODE_SHIFT\020*\022\027\n\023KEY_COD"
  "E_BACK_SLASH\020+\022\016\n\nKEY_CODE_Z\020,\022\016\n\nKEY_CO"
  "DE_X\020-\022\016\n\nKEY_CODE_C\020.\022\016\n\nKEY_CODE_V\020/\022\016"
  "\n\nKEY_CODE_B\0200\022\016\n\nKEY_CODE_N\0201\022\016\n\nKEY_CO"
  "DE_M\0202\022\022\n\016KEY_CODE_COMMA\0203\022\023\n\017KEY_CODE_P"
  "ERIOD\0204\022\020\n\014KEY_CODE_ALT\0208\022\022\n\016KEY_CODE_SP"
  "ACE\0209\022\017\n\013KEY_CODE_F1\020;\022\017\n\013KEY_CODE_F2\020<\022"
  "\017\n\013KEY_CODE_F3\020=\022\017\n\013KEY_CODE_F4\020>\022\017\n\013KEY"
  "_CODE_F5\020\?\022\017\n\013KEY_CODE_F6\020@\022\017\n\013KEY_CODE_"
  "F7\020A\022\017\n\013KEY_CODE_F8\020B\022\017\n\013KEY_CODE_F9\020C\022\020"
  "\n\014KEY_CODE_F10\020D\022\030\n\024KEY_CODE_SCROLL_LOCK"
  "\020F\022\021\n\rKEY_CODE_HOME\020G\022\024\n\020KEY_CODE_PAGE_U"
  "P\020I\022\020\n\014KEY_CODE_END\020O\022\026\n\022KEY_CODE_PAGE_D"
  "OWN\020Q\022\023\n\017KEY_CODE_INSERT\020R\022\023\n\017KEY_CODE_D"
  "ELETE\020S\022\020\n\014KEY_CODE_F11\020W\022\020\n\014KEY_CODE_F1"
  "2\020X*\256\002\n\007KeyType\022\030\n\024KEY_TYPE_UNSPECIFIED\020"
  "\000\022\021\n\rKEY_TYPE_ITEM\020\001\022\022\n\016KEY_TYPE_SKILL\020\002"
  "\022\026\n\022KEY_TYPE_CHAR_INFO\020\003\022\026\n\022KEY_TYPE_EQU"
  "IPMENT\020\004\022\026\n\022KEY_TYPE_INVENTORY\020\005\022\031\n\025KEY_"
  "TYPE_SKILL_WINDOW\020\006\022\023\n\017KEY_TYPE_ATTACK\020\007"
  "\022\021\n\rKEY_TYPE_JUMP\020\010\022\027\n\023KEY_TYPE_KEY_CONF"
  "IG\020\t\022\021\n\rKEY_TYPE_MENU\020\n\022\025\n\021KEY_TYPE_NPC_"
  "TALK\020\013\022\024\n\020KEY_TYPE_PICK_UP\020\014*\316\001\n\rMobActi"
  "onType\022\037\n\033MOB_ACTION_TYPE_UNSPECIFIED\020\000\022"
  "\031\n\025MOB_ACTION_TYPE_STAND\020\001\022\030\n\024MOB_ACTION"
  "_TYPE_MOVE\020\002\022\027\n\023MOB_ACTION_TYPE_HIT\020\003\022\032\n"
  "\026MOB_ACTION_TYPE_ATTACK\020\004\022\027\n\023MOB_ACTION_"
  "TYPE_DIE\020\005\022\031\n\025MOB_ACTION_TYPE_REGEN\020\006b\006p"
  "roto3"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_game_5fenum_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_game_5fenum_2eproto = {
  false, false, 2005, descriptor_table_protodef_game_5fenum_2eproto, "game_enum.proto", 
  &descriptor_table_game_5fenum_2eproto_once, nullptr, 0, 0,
  schemas, file_default_instances, TableStruct_game_5fenum_2eproto::offsets,
  nullptr, file_level_enum_descriptors_game_5fenum_2eproto, file_level_service_descriptors_game_5fenum_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_game_5fenum_2eproto_getter() {
  return &descriptor_table_game_5fenum_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_game_5fenum_2eproto(&descriptor_table_game_5fenum_2eproto);
namespace protocol {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* PlayerAnimation_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_game_5fenum_2eproto);
  return file_level_enum_descriptors_game_5fenum_2eproto[0];
}
bool PlayerAnimation_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* KeyCode_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_game_5fenum_2eproto);
  return file_level_enum_descriptors_game_5fenum_2eproto[1];
}
bool KeyCode_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
    case 49:
    case 50:
    case 51:
    case 52:
    case 56:
    case 57:
    case 59:
    case 60:
    case 61:
    case 62:
    case 63:
    case 64:
    case 65:
    case 66:
    case 67:
    case 68:
    case 70:
    case 71:
    case 73:
    case 79:
    case 81:
    case 82:
    case 83:
    case 87:
    case 88:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* KeyType_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_game_5fenum_2eproto);
  return file_level_enum_descriptors_game_5fenum_2eproto[2];
}
bool KeyType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* MobActionType_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_game_5fenum_2eproto);
  return file_level_enum_descriptors_game_5fenum_2eproto[3];
}
bool MobActionType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace protocol
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
