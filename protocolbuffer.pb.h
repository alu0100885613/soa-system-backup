// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: protocolbuffer.proto

#ifndef PROTOBUF_protocolbuffer_2eproto__INCLUDED
#define PROTOBUF_protocolbuffer_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3002000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3002000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
class BackupMsg;
class BackupMsgDefaultTypeInternal;
extern BackupMsgDefaultTypeInternal _BackupMsg_default_instance_;

namespace protobuf_protocolbuffer_2eproto {
// Internal implementation detail -- do not call these.
struct TableStruct {
  static const ::google::protobuf::uint32 offsets[];
  static void InitDefaultsImpl();
  static void Shutdown();
};
void AddDescriptors();
void InitDefaults();
}  // namespace protobuf_protocolbuffer_2eproto

// ===================================================================

class BackupMsg : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:BackupMsg) */ {
 public:
  BackupMsg();
  virtual ~BackupMsg();

  BackupMsg(const BackupMsg& from);

  inline BackupMsg& operator=(const BackupMsg& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const BackupMsg& default_instance();

  static inline const BackupMsg* internal_default_instance() {
    return reinterpret_cast<const BackupMsg*>(
               &_BackupMsg_default_instance_);
  }

  void Swap(BackupMsg* other);

  // implements Message ----------------------------------------------

  inline BackupMsg* New() const PROTOBUF_FINAL { return New(NULL); }

  BackupMsg* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const BackupMsg& from);
  void MergeFrom(const BackupMsg& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output)
      const PROTOBUF_FINAL {
    return InternalSerializeWithCachedSizesToArray(
        ::google::protobuf::io::CodedOutputStream::IsDefaultSerializationDeterministic(), output);
  }
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(BackupMsg* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string origin_ = 2;
  bool has_origin_() const;
  void clear_origin_();
  static const int kOriginFieldNumber = 2;
  const ::std::string& origin_() const;
  void set_origin_(const ::std::string& value);
  #if LANG_CXX11
  void set_origin_(::std::string&& value);
  #endif
  void set_origin_(const char* value);
  void set_origin_(const char* value, size_t size);
  ::std::string* mutable_origin_();
  ::std::string* release_origin_();
  void set_allocated_origin_(::std::string* origin_);

  // required int32 type_ = 1;
  bool has_type_() const;
  void clear_type_();
  static const int kTypeFieldNumber = 1;
  ::google::protobuf::int32 type_() const;
  void set_type_(::google::protobuf::int32 value);

  // required int32 role_ = 3;
  bool has_role_() const;
  void clear_role_();
  static const int kRoleFieldNumber = 3;
  ::google::protobuf::int32 role_() const;
  void set_role_(::google::protobuf::int32 value);

  // optional int32 nusersAct = 4;
  bool has_nusersact() const;
  void clear_nusersact();
  static const int kNusersActFieldNumber = 4;
  ::google::protobuf::int32 nusersact() const;
  void set_nusersact(::google::protobuf::int32 value);

  // optional int32 nusersPas = 5;
  bool has_nuserspas() const;
  void clear_nuserspas();
  static const int kNusersPasFieldNumber = 5;
  ::google::protobuf::int32 nuserspas() const;
  void set_nuserspas(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:BackupMsg)
 private:
  void set_has_type_();
  void clear_has_type_();
  void set_has_origin_();
  void clear_has_origin_();
  void set_has_role_();
  void clear_has_role_();
  void set_has_nusersact();
  void clear_has_nusersact();
  void set_has_nuserspas();
  void clear_has_nuserspas();

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr origin__;
  ::google::protobuf::int32 type__;
  ::google::protobuf::int32 role__;
  ::google::protobuf::int32 nusersact_;
  ::google::protobuf::int32 nuserspas_;
  friend struct  protobuf_protocolbuffer_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// BackupMsg

// required int32 type_ = 1;
inline bool BackupMsg::has_type_() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void BackupMsg::set_has_type_() {
  _has_bits_[0] |= 0x00000002u;
}
inline void BackupMsg::clear_has_type_() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void BackupMsg::clear_type_() {
  type__ = 0;
  clear_has_type_();
}
inline ::google::protobuf::int32 BackupMsg::type_() const {
  // @@protoc_insertion_point(field_get:BackupMsg.type_)
  return type__;
}
inline void BackupMsg::set_type_(::google::protobuf::int32 value) {
  set_has_type_();
  type__ = value;
  // @@protoc_insertion_point(field_set:BackupMsg.type_)
}

// optional string origin_ = 2;
inline bool BackupMsg::has_origin_() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void BackupMsg::set_has_origin_() {
  _has_bits_[0] |= 0x00000001u;
}
inline void BackupMsg::clear_has_origin_() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void BackupMsg::clear_origin_() {
  origin__.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_origin_();
}
inline const ::std::string& BackupMsg::origin_() const {
  // @@protoc_insertion_point(field_get:BackupMsg.origin_)
  return origin__.GetNoArena();
}
inline void BackupMsg::set_origin_(const ::std::string& value) {
  set_has_origin_();
  origin__.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:BackupMsg.origin_)
}
#if LANG_CXX11
inline void BackupMsg::set_origin_(::std::string&& value) {
  set_has_origin_();
  origin__.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:BackupMsg.origin_)
}
#endif
inline void BackupMsg::set_origin_(const char* value) {
  set_has_origin_();
  origin__.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:BackupMsg.origin_)
}
inline void BackupMsg::set_origin_(const char* value, size_t size) {
  set_has_origin_();
  origin__.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:BackupMsg.origin_)
}
inline ::std::string* BackupMsg::mutable_origin_() {
  set_has_origin_();
  // @@protoc_insertion_point(field_mutable:BackupMsg.origin_)
  return origin__.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* BackupMsg::release_origin_() {
  // @@protoc_insertion_point(field_release:BackupMsg.origin_)
  clear_has_origin_();
  return origin__.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void BackupMsg::set_allocated_origin_(::std::string* origin_) {
  if (origin_ != NULL) {
    set_has_origin_();
  } else {
    clear_has_origin_();
  }
  origin__.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), origin_);
  // @@protoc_insertion_point(field_set_allocated:BackupMsg.origin_)
}

// required int32 role_ = 3;
inline bool BackupMsg::has_role_() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void BackupMsg::set_has_role_() {
  _has_bits_[0] |= 0x00000004u;
}
inline void BackupMsg::clear_has_role_() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void BackupMsg::clear_role_() {
  role__ = 0;
  clear_has_role_();
}
inline ::google::protobuf::int32 BackupMsg::role_() const {
  // @@protoc_insertion_point(field_get:BackupMsg.role_)
  return role__;
}
inline void BackupMsg::set_role_(::google::protobuf::int32 value) {
  set_has_role_();
  role__ = value;
  // @@protoc_insertion_point(field_set:BackupMsg.role_)
}

// optional int32 nusersAct = 4;
inline bool BackupMsg::has_nusersact() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void BackupMsg::set_has_nusersact() {
  _has_bits_[0] |= 0x00000008u;
}
inline void BackupMsg::clear_has_nusersact() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void BackupMsg::clear_nusersact() {
  nusersact_ = 0;
  clear_has_nusersact();
}
inline ::google::protobuf::int32 BackupMsg::nusersact() const {
  // @@protoc_insertion_point(field_get:BackupMsg.nusersAct)
  return nusersact_;
}
inline void BackupMsg::set_nusersact(::google::protobuf::int32 value) {
  set_has_nusersact();
  nusersact_ = value;
  // @@protoc_insertion_point(field_set:BackupMsg.nusersAct)
}

// optional int32 nusersPas = 5;
inline bool BackupMsg::has_nuserspas() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void BackupMsg::set_has_nuserspas() {
  _has_bits_[0] |= 0x00000010u;
}
inline void BackupMsg::clear_has_nuserspas() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void BackupMsg::clear_nuserspas() {
  nuserspas_ = 0;
  clear_has_nuserspas();
}
inline ::google::protobuf::int32 BackupMsg::nuserspas() const {
  // @@protoc_insertion_point(field_get:BackupMsg.nusersPas)
  return nuserspas_;
}
inline void BackupMsg::set_nuserspas(::google::protobuf::int32 value) {
  set_has_nuserspas();
  nuserspas_ = value;
  // @@protoc_insertion_point(field_set:BackupMsg.nusersPas)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_protocolbuffer_2eproto__INCLUDED
