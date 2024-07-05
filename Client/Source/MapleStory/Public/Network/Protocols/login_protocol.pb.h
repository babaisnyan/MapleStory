// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: login_protocol.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_login_5fprotocol_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_login_5fprotocol_2eproto

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
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
#include "login_enum.pb.h"
#include "login_struct.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_login_5fprotocol_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_login_5fprotocol_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[3]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_login_5fprotocol_2eproto;
namespace protocol {
class LoginClientLogin;
struct LoginClientLoginDefaultTypeInternal;
extern LoginClientLoginDefaultTypeInternal _LoginClientLogin_default_instance_;
class LoginServerChat;
struct LoginServerChatDefaultTypeInternal;
extern LoginServerChatDefaultTypeInternal _LoginServerChat_default_instance_;
class LoginServerLogin;
struct LoginServerLoginDefaultTypeInternal;
extern LoginServerLoginDefaultTypeInternal _LoginServerLogin_default_instance_;
}  // namespace protocol
PROTOBUF_NAMESPACE_OPEN
template<> ::protocol::LoginClientLogin* Arena::CreateMaybeMessage<::protocol::LoginClientLogin>(Arena*);
template<> ::protocol::LoginServerChat* Arena::CreateMaybeMessage<::protocol::LoginServerChat>(Arena*);
template<> ::protocol::LoginServerLogin* Arena::CreateMaybeMessage<::protocol::LoginServerLogin>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace protocol {

// ===================================================================

class LoginClientLogin final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:protocol.LoginClientLogin) */ {
 public:
  inline LoginClientLogin() : LoginClientLogin(nullptr) {}
  ~LoginClientLogin() override;
  explicit constexpr LoginClientLogin(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  LoginClientLogin(const LoginClientLogin& from);
  LoginClientLogin(LoginClientLogin&& from) noexcept
    : LoginClientLogin() {
    *this = ::std::move(from);
  }

  inline LoginClientLogin& operator=(const LoginClientLogin& from) {
    CopyFrom(from);
    return *this;
  }
  inline LoginClientLogin& operator=(LoginClientLogin&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const LoginClientLogin& default_instance() {
    return *internal_default_instance();
  }
  static inline const LoginClientLogin* internal_default_instance() {
    return reinterpret_cast<const LoginClientLogin*>(
               &_LoginClientLogin_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(LoginClientLogin& a, LoginClientLogin& b) {
    a.Swap(&b);
  }
  inline void Swap(LoginClientLogin* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(LoginClientLogin* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline LoginClientLogin* New() const final {
    return new LoginClientLogin();
  }

  LoginClientLogin* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<LoginClientLogin>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const LoginClientLogin& from);
  void MergeFrom(const LoginClientLogin& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(LoginClientLogin* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "protocol.LoginClientLogin";
  }
  protected:
  explicit LoginClientLogin(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kUsernameFieldNumber = 1,
    kPasswordFieldNumber = 2,
  };
  // string username = 1;
  void clear_username();
  const std::string& username() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_username(ArgT0&& arg0, ArgT... args);
  std::string* mutable_username();
  PROTOBUF_FUTURE_MUST_USE_RESULT std::string* release_username();
  void set_allocated_username(std::string* username);
  private:
  const std::string& _internal_username() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_username(const std::string& value);
  std::string* _internal_mutable_username();
  public:

  // string password = 2;
  void clear_password();
  const std::string& password() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_password(ArgT0&& arg0, ArgT... args);
  std::string* mutable_password();
  PROTOBUF_FUTURE_MUST_USE_RESULT std::string* release_password();
  void set_allocated_password(std::string* password);
  private:
  const std::string& _internal_password() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_password(const std::string& value);
  std::string* _internal_mutable_password();
  public:

  // @@protoc_insertion_point(class_scope:protocol.LoginClientLogin)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr username_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr password_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_login_5fprotocol_2eproto;
};
// -------------------------------------------------------------------

class LoginServerLogin final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:protocol.LoginServerLogin) */ {
 public:
  inline LoginServerLogin() : LoginServerLogin(nullptr) {}
  ~LoginServerLogin() override;
  explicit constexpr LoginServerLogin(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  LoginServerLogin(const LoginServerLogin& from);
  LoginServerLogin(LoginServerLogin&& from) noexcept
    : LoginServerLogin() {
    *this = ::std::move(from);
  }

  inline LoginServerLogin& operator=(const LoginServerLogin& from) {
    CopyFrom(from);
    return *this;
  }
  inline LoginServerLogin& operator=(LoginServerLogin&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const LoginServerLogin& default_instance() {
    return *internal_default_instance();
  }
  static inline const LoginServerLogin* internal_default_instance() {
    return reinterpret_cast<const LoginServerLogin*>(
               &_LoginServerLogin_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(LoginServerLogin& a, LoginServerLogin& b) {
    a.Swap(&b);
  }
  inline void Swap(LoginServerLogin* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(LoginServerLogin* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline LoginServerLogin* New() const final {
    return new LoginServerLogin();
  }

  LoginServerLogin* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<LoginServerLogin>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const LoginServerLogin& from);
  void MergeFrom(const LoginServerLogin& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(LoginServerLogin* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "protocol.LoginServerLogin";
  }
  protected:
  explicit LoginServerLogin(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kResultFieldNumber = 1,
  };
  // .protocol.LoginResult result = 1;
  void clear_result();
  ::protocol::LoginResult result() const;
  void set_result(::protocol::LoginResult value);
  private:
  ::protocol::LoginResult _internal_result() const;
  void _internal_set_result(::protocol::LoginResult value);
  public:

  // @@protoc_insertion_point(class_scope:protocol.LoginServerLogin)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  int result_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_login_5fprotocol_2eproto;
};
// -------------------------------------------------------------------

class LoginServerChat final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:protocol.LoginServerChat) */ {
 public:
  inline LoginServerChat() : LoginServerChat(nullptr) {}
  ~LoginServerChat() override;
  explicit constexpr LoginServerChat(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  LoginServerChat(const LoginServerChat& from);
  LoginServerChat(LoginServerChat&& from) noexcept
    : LoginServerChat() {
    *this = ::std::move(from);
  }

  inline LoginServerChat& operator=(const LoginServerChat& from) {
    CopyFrom(from);
    return *this;
  }
  inline LoginServerChat& operator=(LoginServerChat&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const LoginServerChat& default_instance() {
    return *internal_default_instance();
  }
  static inline const LoginServerChat* internal_default_instance() {
    return reinterpret_cast<const LoginServerChat*>(
               &_LoginServerChat_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(LoginServerChat& a, LoginServerChat& b) {
    a.Swap(&b);
  }
  inline void Swap(LoginServerChat* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(LoginServerChat* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline LoginServerChat* New() const final {
    return new LoginServerChat();
  }

  LoginServerChat* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<LoginServerChat>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const LoginServerChat& from);
  void MergeFrom(const LoginServerChat& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(LoginServerChat* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "protocol.LoginServerChat";
  }
  protected:
  explicit LoginServerChat(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kMessageFieldNumber = 1,
  };
  // string message = 1;
  void clear_message();
  const std::string& message() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_message(ArgT0&& arg0, ArgT... args);
  std::string* mutable_message();
  PROTOBUF_FUTURE_MUST_USE_RESULT std::string* release_message();
  void set_allocated_message(std::string* message);
  private:
  const std::string& _internal_message() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_message(const std::string& value);
  std::string* _internal_mutable_message();
  public:

  // @@protoc_insertion_point(class_scope:protocol.LoginServerChat)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr message_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_login_5fprotocol_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// LoginClientLogin

// string username = 1;
inline void LoginClientLogin::clear_username() {
  username_.ClearToEmpty();
}
inline const std::string& LoginClientLogin::username() const {
  // @@protoc_insertion_point(field_get:protocol.LoginClientLogin.username)
  return _internal_username();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void LoginClientLogin::set_username(ArgT0&& arg0, ArgT... args) {
 
 username_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:protocol.LoginClientLogin.username)
}
inline std::string* LoginClientLogin::mutable_username() {
  // @@protoc_insertion_point(field_mutable:protocol.LoginClientLogin.username)
  return _internal_mutable_username();
}
inline const std::string& LoginClientLogin::_internal_username() const {
  return username_.Get();
}
inline void LoginClientLogin::_internal_set_username(const std::string& value) {
  
  username_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* LoginClientLogin::_internal_mutable_username() {
  
  return username_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* LoginClientLogin::release_username() {
  // @@protoc_insertion_point(field_release:protocol.LoginClientLogin.username)
  return username_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void LoginClientLogin::set_allocated_username(std::string* username) {
  if (username != nullptr) {
    
  } else {
    
  }
  username_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), username,
      GetArenaForAllocation());
  // @@protoc_insertion_point(field_set_allocated:protocol.LoginClientLogin.username)
}

// string password = 2;
inline void LoginClientLogin::clear_password() {
  password_.ClearToEmpty();
}
inline const std::string& LoginClientLogin::password() const {
  // @@protoc_insertion_point(field_get:protocol.LoginClientLogin.password)
  return _internal_password();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void LoginClientLogin::set_password(ArgT0&& arg0, ArgT... args) {
 
 password_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:protocol.LoginClientLogin.password)
}
inline std::string* LoginClientLogin::mutable_password() {
  // @@protoc_insertion_point(field_mutable:protocol.LoginClientLogin.password)
  return _internal_mutable_password();
}
inline const std::string& LoginClientLogin::_internal_password() const {
  return password_.Get();
}
inline void LoginClientLogin::_internal_set_password(const std::string& value) {
  
  password_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* LoginClientLogin::_internal_mutable_password() {
  
  return password_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* LoginClientLogin::release_password() {
  // @@protoc_insertion_point(field_release:protocol.LoginClientLogin.password)
  return password_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void LoginClientLogin::set_allocated_password(std::string* password) {
  if (password != nullptr) {
    
  } else {
    
  }
  password_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), password,
      GetArenaForAllocation());
  // @@protoc_insertion_point(field_set_allocated:protocol.LoginClientLogin.password)
}

// -------------------------------------------------------------------

// LoginServerLogin

// .protocol.LoginResult result = 1;
inline void LoginServerLogin::clear_result() {
  result_ = 0;
}
inline ::protocol::LoginResult LoginServerLogin::_internal_result() const {
  return static_cast< ::protocol::LoginResult >(result_);
}
inline ::protocol::LoginResult LoginServerLogin::result() const {
  // @@protoc_insertion_point(field_get:protocol.LoginServerLogin.result)
  return _internal_result();
}
inline void LoginServerLogin::_internal_set_result(::protocol::LoginResult value) {
  
  result_ = value;
}
inline void LoginServerLogin::set_result(::protocol::LoginResult value) {
  _internal_set_result(value);
  // @@protoc_insertion_point(field_set:protocol.LoginServerLogin.result)
}

// -------------------------------------------------------------------

// LoginServerChat

// string message = 1;
inline void LoginServerChat::clear_message() {
  message_.ClearToEmpty();
}
inline const std::string& LoginServerChat::message() const {
  // @@protoc_insertion_point(field_get:protocol.LoginServerChat.message)
  return _internal_message();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void LoginServerChat::set_message(ArgT0&& arg0, ArgT... args) {
 
 message_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:protocol.LoginServerChat.message)
}
inline std::string* LoginServerChat::mutable_message() {
  // @@protoc_insertion_point(field_mutable:protocol.LoginServerChat.message)
  return _internal_mutable_message();
}
inline const std::string& LoginServerChat::_internal_message() const {
  return message_.Get();
}
inline void LoginServerChat::_internal_set_message(const std::string& value) {
  
  message_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* LoginServerChat::_internal_mutable_message() {
  
  return message_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* LoginServerChat::release_message() {
  // @@protoc_insertion_point(field_release:protocol.LoginServerChat.message)
  return message_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void LoginServerChat::set_allocated_message(std::string* message) {
  if (message != nullptr) {
    
  } else {
    
  }
  message_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), message,
      GetArenaForAllocation());
  // @@protoc_insertion_point(field_set_allocated:protocol.LoginServerChat.message)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace protocol

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_login_5fprotocol_2eproto
