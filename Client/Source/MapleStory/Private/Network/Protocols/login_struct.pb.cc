// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: login_struct.proto

#include "login_struct.pb.h"

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
constexpr LoginCharacter::LoginCharacter(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : name_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , id_(0)
  , level_(0)
  , type_(0)
  , job_(0)
  , str_(0)
  , dex_(0)
  , luk_(0)
  , int__(0){}
struct LoginCharacterDefaultTypeInternal {
  constexpr LoginCharacterDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~LoginCharacterDefaultTypeInternal() {}
  union {
    LoginCharacter _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT LoginCharacterDefaultTypeInternal _LoginCharacter_default_instance_;
}  // namespace protocol
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_login_5fstruct_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_login_5fstruct_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_login_5fstruct_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_login_5fstruct_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::protocol::LoginCharacter, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::protocol::LoginCharacter, id_),
  PROTOBUF_FIELD_OFFSET(::protocol::LoginCharacter, name_),
  PROTOBUF_FIELD_OFFSET(::protocol::LoginCharacter, level_),
  PROTOBUF_FIELD_OFFSET(::protocol::LoginCharacter, type_),
  PROTOBUF_FIELD_OFFSET(::protocol::LoginCharacter, job_),
  PROTOBUF_FIELD_OFFSET(::protocol::LoginCharacter, str_),
  PROTOBUF_FIELD_OFFSET(::protocol::LoginCharacter, dex_),
  PROTOBUF_FIELD_OFFSET(::protocol::LoginCharacter, luk_),
  PROTOBUF_FIELD_OFFSET(::protocol::LoginCharacter, int__),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::protocol::LoginCharacter)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::protocol::_LoginCharacter_default_instance_),
};

const char descriptor_table_protodef_login_5fstruct_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\022login_struct.proto\022\010protocol\032\020login_en"
  "um.proto\"\210\001\n\016LoginCharacter\022\n\n\002id\030\001 \001(\005\022"
  "\014\n\004name\030\002 \001(\t\022\r\n\005level\030\003 \001(\005\022\014\n\004type\030\004 \001"
  "(\005\022\013\n\003job\030\005 \001(\005\022\013\n\003str\030\006 \001(\005\022\013\n\003dex\030\007 \001("
  "\005\022\013\n\003luk\030\010 \001(\005\022\013\n\003int\030\t \001(\005b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_login_5fstruct_2eproto_deps[1] = {
  &::descriptor_table_login_5fenum_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_login_5fstruct_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_login_5fstruct_2eproto = {
  false, false, 195, descriptor_table_protodef_login_5fstruct_2eproto, "login_struct.proto", 
  &descriptor_table_login_5fstruct_2eproto_once, descriptor_table_login_5fstruct_2eproto_deps, 1, 1,
  schemas, file_default_instances, TableStruct_login_5fstruct_2eproto::offsets,
  file_level_metadata_login_5fstruct_2eproto, file_level_enum_descriptors_login_5fstruct_2eproto, file_level_service_descriptors_login_5fstruct_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_login_5fstruct_2eproto_getter() {
  return &descriptor_table_login_5fstruct_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_login_5fstruct_2eproto(&descriptor_table_login_5fstruct_2eproto);
namespace protocol {

// ===================================================================

class LoginCharacter::_Internal {
 public:
};

LoginCharacter::LoginCharacter(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:protocol.LoginCharacter)
}
LoginCharacter::LoginCharacter(const LoginCharacter& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  name_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_name().empty()) {
    name_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_name(), 
      GetArenaForAllocation());
  }
  ::memcpy(&id_, &from.id_,
    static_cast<size_t>(reinterpret_cast<char*>(&int__) -
    reinterpret_cast<char*>(&id_)) + sizeof(int__));
  // @@protoc_insertion_point(copy_constructor:protocol.LoginCharacter)
}

void LoginCharacter::SharedCtor() {
name_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&id_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&int__) -
    reinterpret_cast<char*>(&id_)) + sizeof(int__));
}

LoginCharacter::~LoginCharacter() {
  // @@protoc_insertion_point(destructor:protocol.LoginCharacter)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void LoginCharacter::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  name_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void LoginCharacter::ArenaDtor(void* object) {
  LoginCharacter* _this = reinterpret_cast< LoginCharacter* >(object);
  (void)_this;
}
void LoginCharacter::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void LoginCharacter::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void LoginCharacter::Clear() {
// @@protoc_insertion_point(message_clear_start:protocol.LoginCharacter)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  name_.ClearToEmpty();
  ::memset(&id_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&int__) -
      reinterpret_cast<char*>(&id_)) + sizeof(int__));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* LoginCharacter::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int32 id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string name = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          auto str = _internal_mutable_name();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "protocol.LoginCharacter.name"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 level = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          level_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 type = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 32)) {
          type_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 job = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 40)) {
          job_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 str = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 48)) {
          str_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 dex = 7;
      case 7:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 56)) {
          dex_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 luk = 8;
      case 8:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 64)) {
          luk_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 int = 9;
      case 9:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 72)) {
          int__ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag == 0) || ((tag & 7) == 4)) {
          CHK_(ptr);
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* LoginCharacter::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:protocol.LoginCharacter)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 id = 1;
  if (this->id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->_internal_id(), target);
  }

  // string name = 2;
  if (!this->name().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_name().data(), static_cast<int>(this->_internal_name().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "protocol.LoginCharacter.name");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_name(), target);
  }

  // int32 level = 3;
  if (this->level() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(3, this->_internal_level(), target);
  }

  // int32 type = 4;
  if (this->type() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(4, this->_internal_type(), target);
  }

  // int32 job = 5;
  if (this->job() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(5, this->_internal_job(), target);
  }

  // int32 str = 6;
  if (this->str() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(6, this->_internal_str(), target);
  }

  // int32 dex = 7;
  if (this->dex() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(7, this->_internal_dex(), target);
  }

  // int32 luk = 8;
  if (this->luk() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(8, this->_internal_luk(), target);
  }

  // int32 int = 9;
  if (this->int_() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(9, this->_internal_int_(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:protocol.LoginCharacter)
  return target;
}

size_t LoginCharacter::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:protocol.LoginCharacter)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string name = 2;
  if (!this->name().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_name());
  }

  // int32 id = 1;
  if (this->id() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_id());
  }

  // int32 level = 3;
  if (this->level() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_level());
  }

  // int32 type = 4;
  if (this->type() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_type());
  }

  // int32 job = 5;
  if (this->job() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_job());
  }

  // int32 str = 6;
  if (this->str() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_str());
  }

  // int32 dex = 7;
  if (this->dex() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_dex());
  }

  // int32 luk = 8;
  if (this->luk() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_luk());
  }

  // int32 int = 9;
  if (this->int_() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_int_());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void LoginCharacter::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:protocol.LoginCharacter)
  GOOGLE_DCHECK_NE(&from, this);
  const LoginCharacter* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<LoginCharacter>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:protocol.LoginCharacter)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:protocol.LoginCharacter)
    MergeFrom(*source);
  }
}

void LoginCharacter::MergeFrom(const LoginCharacter& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:protocol.LoginCharacter)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from.name().empty()) {
    _internal_set_name(from._internal_name());
  }
  if (from.id() != 0) {
    _internal_set_id(from._internal_id());
  }
  if (from.level() != 0) {
    _internal_set_level(from._internal_level());
  }
  if (from.type() != 0) {
    _internal_set_type(from._internal_type());
  }
  if (from.job() != 0) {
    _internal_set_job(from._internal_job());
  }
  if (from.str() != 0) {
    _internal_set_str(from._internal_str());
  }
  if (from.dex() != 0) {
    _internal_set_dex(from._internal_dex());
  }
  if (from.luk() != 0) {
    _internal_set_luk(from._internal_luk());
  }
  if (from.int_() != 0) {
    _internal_set_int_(from._internal_int_());
  }
}

void LoginCharacter::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:protocol.LoginCharacter)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void LoginCharacter::CopyFrom(const LoginCharacter& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:protocol.LoginCharacter)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool LoginCharacter::IsInitialized() const {
  return true;
}

void LoginCharacter::InternalSwap(LoginCharacter* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &name_, GetArenaForAllocation(),
      &other->name_, other->GetArenaForAllocation()
  );
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(LoginCharacter, int__)
      + sizeof(LoginCharacter::int__)
      - PROTOBUF_FIELD_OFFSET(LoginCharacter, id_)>(
          reinterpret_cast<char*>(&id_),
          reinterpret_cast<char*>(&other->id_));
}

::PROTOBUF_NAMESPACE_ID::Metadata LoginCharacter::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_login_5fstruct_2eproto_getter, &descriptor_table_login_5fstruct_2eproto_once,
      file_level_metadata_login_5fstruct_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace protocol
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::protocol::LoginCharacter* Arena::CreateMaybeMessage< ::protocol::LoginCharacter >(Arena* arena) {
  return Arena::CreateMessageInternal< ::protocol::LoginCharacter >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
