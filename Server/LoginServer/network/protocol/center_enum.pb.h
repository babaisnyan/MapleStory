// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: center_enum.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_center_5fenum_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_center_5fenum_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_center_5fenum_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_center_5fenum_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_center_5fenum_2eproto;
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE
namespace protocol {

enum ServerType : int {
  SERVER_TYPE_UNSPECIFIED = 0,
  SERVER_TYPE_CENTER = 1,
  SERVER_TYPE_LOGIN = 2,
  SERVER_TYPE_GAME = 3,
  ServerType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  ServerType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool ServerType_IsValid(int value);
constexpr ServerType ServerType_MIN = SERVER_TYPE_UNSPECIFIED;
constexpr ServerType ServerType_MAX = SERVER_TYPE_GAME;
constexpr int ServerType_ARRAYSIZE = ServerType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ServerType_descriptor();
template<typename T>
inline const std::string& ServerType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, ServerType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function ServerType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    ServerType_descriptor(), enum_t_value);
}
inline bool ServerType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, ServerType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<ServerType>(
    ServerType_descriptor(), name, value);
}
enum ServerRegisterResult : int {
  SERVER_REGISTER_RESULT_UNSPECIFIED = 0,
  SERVER_REGISTER_RESULT_SUCCESS = 1,
  SERVER_REGISTER_RESULT_ALREADY_REGISTERED = 2,
  SERVER_REGISTER_RESULT_INVALID_TYPE = 3,
  SERVER_REGISTER_RESULT_INVALID_VERSION = 4,
  SERVER_REGISTER_RESULT_INVALID_NAME = 5,
  SERVER_REGISTER_RESULT_INVALID_IP = 6,
  SERVER_REGISTER_RESULT_INVALID_PORT = 7,
  SERVER_REGISTER_RESULT_ERROR = 8,
  ServerRegisterResult_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  ServerRegisterResult_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool ServerRegisterResult_IsValid(int value);
constexpr ServerRegisterResult ServerRegisterResult_MIN = SERVER_REGISTER_RESULT_UNSPECIFIED;
constexpr ServerRegisterResult ServerRegisterResult_MAX = SERVER_REGISTER_RESULT_ERROR;
constexpr int ServerRegisterResult_ARRAYSIZE = ServerRegisterResult_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ServerRegisterResult_descriptor();
template<typename T>
inline const std::string& ServerRegisterResult_Name(T enum_t_value) {
  static_assert(::std::is_same<T, ServerRegisterResult>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function ServerRegisterResult_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    ServerRegisterResult_descriptor(), enum_t_value);
}
inline bool ServerRegisterResult_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, ServerRegisterResult* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<ServerRegisterResult>(
    ServerRegisterResult_descriptor(), name, value);
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

template <> struct is_proto_enum< ::protocol::ServerType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::protocol::ServerType>() {
  return ::protocol::ServerType_descriptor();
}
template <> struct is_proto_enum< ::protocol::ServerRegisterResult> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::protocol::ServerRegisterResult>() {
  return ::protocol::ServerRegisterResult_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_center_5fenum_2eproto
