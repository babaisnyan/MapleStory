// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: login_enum.proto

#include "login_enum.pb.h"

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
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_login_5fenum_2eproto[2];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_login_5fenum_2eproto = nullptr;
const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_login_5fenum_2eproto::offsets[1] = {};
static constexpr ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema* schemas = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_login_5fenum_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\020login_enum.proto\022\010protocol*j\n\013LoginRes"
  "ult\022\013\n\007Success\020\000\022\023\n\017InvalidUsername\020\001\022\023\n"
  "\017InvalidPassword\020\002\022\023\n\017AlreadyLoggedIn\020\003\022"
  "\017\n\013ServerError\020\004*_\n\020CreateCharResult\022\025\n\021"
  "CreateCharSuccess\020\000\022\031\n\025CreateCharInvalid"
  "Name\020\001\022\031\n\025CreateCharServerError\020\002b\006proto"
  "3"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_login_5fenum_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_login_5fenum_2eproto = {
  false, false, 241, descriptor_table_protodef_login_5fenum_2eproto, "login_enum.proto", 
  &descriptor_table_login_5fenum_2eproto_once, nullptr, 0, 0,
  schemas, file_default_instances, TableStruct_login_5fenum_2eproto::offsets,
  nullptr, file_level_enum_descriptors_login_5fenum_2eproto, file_level_service_descriptors_login_5fenum_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_login_5fenum_2eproto_getter() {
  return &descriptor_table_login_5fenum_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_login_5fenum_2eproto(&descriptor_table_login_5fenum_2eproto);
namespace protocol {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* LoginResult_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_login_5fenum_2eproto);
  return file_level_enum_descriptors_login_5fenum_2eproto[0];
}
bool LoginResult_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* CreateCharResult_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_login_5fenum_2eproto);
  return file_level_enum_descriptors_login_5fenum_2eproto[1];
}
bool CreateCharResult_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
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
