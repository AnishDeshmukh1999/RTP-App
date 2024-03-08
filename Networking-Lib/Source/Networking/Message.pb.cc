// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Message.proto

#include "Message.pb.h"

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

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace Message {
PROTOBUF_CONSTEXPR Request::Request(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.type_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct RequestDefaultTypeInternal {
  PROTOBUF_CONSTEXPR RequestDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~RequestDefaultTypeInternal() {}
  union {
    Request _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 RequestDefaultTypeInternal _Request_default_instance_;
PROTOBUF_CONSTEXPR ID3v2Tag::ID3v2Tag(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.identifier_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.data_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.majorversion_)*/0u
  , /*decltype(_impl_.minorversion_)*/0u
  , /*decltype(_impl_.flags_)*/0u
  , /*decltype(_impl_.s_tagsize_)*/0u
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct ID3v2TagDefaultTypeInternal {
  PROTOBUF_CONSTEXPR ID3v2TagDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~ID3v2TagDefaultTypeInternal() {}
  union {
    ID3v2Tag _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 ID3v2TagDefaultTypeInternal _ID3v2Tag_default_instance_;
PROTOBUF_CONSTEXPR Response::Response(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.data_)*/{}
  , /*decltype(_impl_._cached_size_)*/{}
  , /*decltype(_impl_._oneof_case_)*/{}} {}
struct ResponseDefaultTypeInternal {
  PROTOBUF_CONSTEXPR ResponseDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~ResponseDefaultTypeInternal() {}
  union {
    Response _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 ResponseDefaultTypeInternal _Response_default_instance_;
}  // namespace Message
static ::_pb::Metadata file_level_metadata_Message_2eproto[3];
static const ::_pb::EnumDescriptor* file_level_enum_descriptors_Message_2eproto[2];
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_Message_2eproto = nullptr;

const uint32_t TableStruct_Message_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::Message::Request, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::Message::Request, _impl_.type_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::Message::ID3v2Tag, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::Message::ID3v2Tag, _impl_.identifier_),
  PROTOBUF_FIELD_OFFSET(::Message::ID3v2Tag, _impl_.majorversion_),
  PROTOBUF_FIELD_OFFSET(::Message::ID3v2Tag, _impl_.minorversion_),
  PROTOBUF_FIELD_OFFSET(::Message::ID3v2Tag, _impl_.flags_),
  PROTOBUF_FIELD_OFFSET(::Message::ID3v2Tag, _impl_.s_tagsize_),
  PROTOBUF_FIELD_OFFSET(::Message::ID3v2Tag, _impl_.data_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::Message::Response, _internal_metadata_),
  ~0u,  // no _extensions_
  PROTOBUF_FIELD_OFFSET(::Message::Response, _impl_._oneof_case_[0]),
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  ::_pbi::kInvalidFieldOffsetTag,
  ::_pbi::kInvalidFieldOffsetTag,
  PROTOBUF_FIELD_OFFSET(::Message::Response, _impl_.data_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::Message::Request)},
  { 7, -1, -1, sizeof(::Message::ID3v2Tag)},
  { 19, -1, -1, sizeof(::Message::Response)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::Message::_Request_default_instance_._instance,
  &::Message::_ID3v2Tag_default_instance_._instance,
  &::Message::_Response_default_instance_._instance,
};

const char descriptor_table_protodef_Message_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\rMessage.proto\022\007Message\"\213\001\n\007Request\022*\n\004"
  "type\030\001 \001(\0162\034.Message.Request.RequestType"
  "\"T\n\013RequestType\022\013\n\007DEFAULT\020\000\022\017\n\013GET_ID3_"
  "TAG\020\001\022\023\n\017START_STREAMING\020\002\022\022\n\016STOP_STREA"
  "MING\020\003\"z\n\010ID3v2Tag\022\022\n\nidentifier\030\001 \001(\014\022\024"
  "\n\014majorversion\030\002 \001(\r\022\024\n\014minorversion\030\003 \001"
  "(\r\022\r\n\005flags\030\004 \001(\r\022\021\n\ts_tagsize\030\005 \001(\r\022\014\n\004"
  "data\030\006 \001(\014\"\212\001\n\010Response\022%\n\010id3v2Tag\030\001 \001("
  "\0132\021.Message.ID3v2TagH\000\0222\n\010response\030\002 \001(\016"
  "2\036.Message.Response.ResponseTypeH\000\"\033\n\014Re"
  "sponseType\022\013\n\007SUCCESS\020\000B\006\n\004datab\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_Message_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_Message_2eproto = {
    false, false, 439, descriptor_table_protodef_Message_2eproto,
    "Message.proto",
    &descriptor_table_Message_2eproto_once, nullptr, 0, 3,
    schemas, file_default_instances, TableStruct_Message_2eproto::offsets,
    file_level_metadata_Message_2eproto, file_level_enum_descriptors_Message_2eproto,
    file_level_service_descriptors_Message_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_Message_2eproto_getter() {
  return &descriptor_table_Message_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_Message_2eproto(&descriptor_table_Message_2eproto);
namespace Message {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Request_RequestType_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_Message_2eproto);
  return file_level_enum_descriptors_Message_2eproto[0];
}
bool Request_RequestType_IsValid(int value) {
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

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || (_MSC_VER >= 1900 && _MSC_VER < 1912))
constexpr Request_RequestType Request::DEFAULT;
constexpr Request_RequestType Request::GET_ID3_TAG;
constexpr Request_RequestType Request::START_STREAMING;
constexpr Request_RequestType Request::STOP_STREAMING;
constexpr Request_RequestType Request::RequestType_MIN;
constexpr Request_RequestType Request::RequestType_MAX;
constexpr int Request::RequestType_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || (_MSC_VER >= 1900 && _MSC_VER < 1912))
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Response_ResponseType_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_Message_2eproto);
  return file_level_enum_descriptors_Message_2eproto[1];
}
bool Response_ResponseType_IsValid(int value) {
  switch (value) {
    case 0:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || (_MSC_VER >= 1900 && _MSC_VER < 1912))
constexpr Response_ResponseType Response::SUCCESS;
constexpr Response_ResponseType Response::ResponseType_MIN;
constexpr Response_ResponseType Response::ResponseType_MAX;
constexpr int Response::ResponseType_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || (_MSC_VER >= 1900 && _MSC_VER < 1912))

// ===================================================================

class Request::_Internal {
 public:
};

Request::Request(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:Message.Request)
}
Request::Request(const Request& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  Request* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.type_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _this->_impl_.type_ = from._impl_.type_;
  // @@protoc_insertion_point(copy_constructor:Message.Request)
}

inline void Request::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.type_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

Request::~Request() {
  // @@protoc_insertion_point(destructor:Message.Request)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void Request::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void Request::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void Request::Clear() {
// @@protoc_insertion_point(message_clear_start:Message.Request)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.type_ = 0;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Request::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // .Message.Request.RequestType type = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          uint64_t val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_type(static_cast<::Message::Request_RequestType>(val));
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* Request::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:Message.Request)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // .Message.Request.RequestType type = 1;
  if (this->_internal_type() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteEnumToArray(
      1, this->_internal_type(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Message.Request)
  return target;
}

size_t Request::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Message.Request)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // .Message.Request.RequestType type = 1;
  if (this->_internal_type() != 0) {
    total_size += 1 +
      ::_pbi::WireFormatLite::EnumSize(this->_internal_type());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData Request::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    Request::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*Request::GetClassData() const { return &_class_data_; }


void Request::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<Request*>(&to_msg);
  auto& from = static_cast<const Request&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:Message.Request)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_type() != 0) {
    _this->_internal_set_type(from._internal_type());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void Request::CopyFrom(const Request& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Message.Request)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Request::IsInitialized() const {
  return true;
}

void Request::InternalSwap(Request* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_.type_, other->_impl_.type_);
}

::PROTOBUF_NAMESPACE_ID::Metadata Request::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_Message_2eproto_getter, &descriptor_table_Message_2eproto_once,
      file_level_metadata_Message_2eproto[0]);
}

// ===================================================================

class ID3v2Tag::_Internal {
 public:
};

ID3v2Tag::ID3v2Tag(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:Message.ID3v2Tag)
}
ID3v2Tag::ID3v2Tag(const ID3v2Tag& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  ID3v2Tag* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.identifier_){}
    , decltype(_impl_.data_){}
    , decltype(_impl_.majorversion_){}
    , decltype(_impl_.minorversion_){}
    , decltype(_impl_.flags_){}
    , decltype(_impl_.s_tagsize_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.identifier_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.identifier_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_identifier().empty()) {
    _this->_impl_.identifier_.Set(from._internal_identifier(), 
      _this->GetArenaForAllocation());
  }
  _impl_.data_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.data_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_data().empty()) {
    _this->_impl_.data_.Set(from._internal_data(), 
      _this->GetArenaForAllocation());
  }
  ::memcpy(&_impl_.majorversion_, &from._impl_.majorversion_,
    static_cast<size_t>(reinterpret_cast<char*>(&_impl_.s_tagsize_) -
    reinterpret_cast<char*>(&_impl_.majorversion_)) + sizeof(_impl_.s_tagsize_));
  // @@protoc_insertion_point(copy_constructor:Message.ID3v2Tag)
}

inline void ID3v2Tag::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.identifier_){}
    , decltype(_impl_.data_){}
    , decltype(_impl_.majorversion_){0u}
    , decltype(_impl_.minorversion_){0u}
    , decltype(_impl_.flags_){0u}
    , decltype(_impl_.s_tagsize_){0u}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.identifier_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.identifier_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  _impl_.data_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.data_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

ID3v2Tag::~ID3v2Tag() {
  // @@protoc_insertion_point(destructor:Message.ID3v2Tag)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void ID3v2Tag::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.identifier_.Destroy();
  _impl_.data_.Destroy();
}

void ID3v2Tag::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void ID3v2Tag::Clear() {
// @@protoc_insertion_point(message_clear_start:Message.ID3v2Tag)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.identifier_.ClearToEmpty();
  _impl_.data_.ClearToEmpty();
  ::memset(&_impl_.majorversion_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&_impl_.s_tagsize_) -
      reinterpret_cast<char*>(&_impl_.majorversion_)) + sizeof(_impl_.s_tagsize_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* ID3v2Tag::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // bytes identifier = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_identifier();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // uint32 majorversion = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
          _impl_.majorversion_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // uint32 minorversion = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 24)) {
          _impl_.minorversion_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // uint32 flags = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 32)) {
          _impl_.flags_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // uint32 s_tagsize = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 40)) {
          _impl_.s_tagsize_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // bytes data = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 50)) {
          auto str = _internal_mutable_data();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* ID3v2Tag::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:Message.ID3v2Tag)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // bytes identifier = 1;
  if (!this->_internal_identifier().empty()) {
    target = stream->WriteBytesMaybeAliased(
        1, this->_internal_identifier(), target);
  }

  // uint32 majorversion = 2;
  if (this->_internal_majorversion() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt32ToArray(2, this->_internal_majorversion(), target);
  }

  // uint32 minorversion = 3;
  if (this->_internal_minorversion() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt32ToArray(3, this->_internal_minorversion(), target);
  }

  // uint32 flags = 4;
  if (this->_internal_flags() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt32ToArray(4, this->_internal_flags(), target);
  }

  // uint32 s_tagsize = 5;
  if (this->_internal_s_tagsize() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt32ToArray(5, this->_internal_s_tagsize(), target);
  }

  // bytes data = 6;
  if (!this->_internal_data().empty()) {
    target = stream->WriteBytesMaybeAliased(
        6, this->_internal_data(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Message.ID3v2Tag)
  return target;
}

size_t ID3v2Tag::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Message.ID3v2Tag)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // bytes identifier = 1;
  if (!this->_internal_identifier().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_identifier());
  }

  // bytes data = 6;
  if (!this->_internal_data().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_data());
  }

  // uint32 majorversion = 2;
  if (this->_internal_majorversion() != 0) {
    total_size += ::_pbi::WireFormatLite::UInt32SizePlusOne(this->_internal_majorversion());
  }

  // uint32 minorversion = 3;
  if (this->_internal_minorversion() != 0) {
    total_size += ::_pbi::WireFormatLite::UInt32SizePlusOne(this->_internal_minorversion());
  }

  // uint32 flags = 4;
  if (this->_internal_flags() != 0) {
    total_size += ::_pbi::WireFormatLite::UInt32SizePlusOne(this->_internal_flags());
  }

  // uint32 s_tagsize = 5;
  if (this->_internal_s_tagsize() != 0) {
    total_size += ::_pbi::WireFormatLite::UInt32SizePlusOne(this->_internal_s_tagsize());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData ID3v2Tag::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    ID3v2Tag::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*ID3v2Tag::GetClassData() const { return &_class_data_; }


void ID3v2Tag::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<ID3v2Tag*>(&to_msg);
  auto& from = static_cast<const ID3v2Tag&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:Message.ID3v2Tag)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_identifier().empty()) {
    _this->_internal_set_identifier(from._internal_identifier());
  }
  if (!from._internal_data().empty()) {
    _this->_internal_set_data(from._internal_data());
  }
  if (from._internal_majorversion() != 0) {
    _this->_internal_set_majorversion(from._internal_majorversion());
  }
  if (from._internal_minorversion() != 0) {
    _this->_internal_set_minorversion(from._internal_minorversion());
  }
  if (from._internal_flags() != 0) {
    _this->_internal_set_flags(from._internal_flags());
  }
  if (from._internal_s_tagsize() != 0) {
    _this->_internal_set_s_tagsize(from._internal_s_tagsize());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void ID3v2Tag::CopyFrom(const ID3v2Tag& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Message.ID3v2Tag)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ID3v2Tag::IsInitialized() const {
  return true;
}

void ID3v2Tag::InternalSwap(ID3v2Tag* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.identifier_, lhs_arena,
      &other->_impl_.identifier_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.data_, lhs_arena,
      &other->_impl_.data_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(ID3v2Tag, _impl_.s_tagsize_)
      + sizeof(ID3v2Tag::_impl_.s_tagsize_)
      - PROTOBUF_FIELD_OFFSET(ID3v2Tag, _impl_.majorversion_)>(
          reinterpret_cast<char*>(&_impl_.majorversion_),
          reinterpret_cast<char*>(&other->_impl_.majorversion_));
}

::PROTOBUF_NAMESPACE_ID::Metadata ID3v2Tag::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_Message_2eproto_getter, &descriptor_table_Message_2eproto_once,
      file_level_metadata_Message_2eproto[1]);
}

// ===================================================================

class Response::_Internal {
 public:
  static const ::Message::ID3v2Tag& id3v2tag(const Response* msg);
};

const ::Message::ID3v2Tag&
Response::_Internal::id3v2tag(const Response* msg) {
  return *msg->_impl_.data_.id3v2tag_;
}
void Response::set_allocated_id3v2tag(::Message::ID3v2Tag* id3v2tag) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaForAllocation();
  clear_data();
  if (id3v2tag) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      ::PROTOBUF_NAMESPACE_ID::Arena::InternalGetOwningArena(id3v2tag);
    if (message_arena != submessage_arena) {
      id3v2tag = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, id3v2tag, submessage_arena);
    }
    set_has_id3v2tag();
    _impl_.data_.id3v2tag_ = id3v2tag;
  }
  // @@protoc_insertion_point(field_set_allocated:Message.Response.id3v2Tag)
}
Response::Response(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:Message.Response)
}
Response::Response(const Response& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  Response* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.data_){}
    , /*decltype(_impl_._cached_size_)*/{}
    , /*decltype(_impl_._oneof_case_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  clear_has_data();
  switch (from.data_case()) {
    case kId3V2Tag: {
      _this->_internal_mutable_id3v2tag()->::Message::ID3v2Tag::MergeFrom(
          from._internal_id3v2tag());
      break;
    }
    case kResponse: {
      _this->_internal_set_response(from._internal_response());
      break;
    }
    case DATA_NOT_SET: {
      break;
    }
  }
  // @@protoc_insertion_point(copy_constructor:Message.Response)
}

inline void Response::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.data_){}
    , /*decltype(_impl_._cached_size_)*/{}
    , /*decltype(_impl_._oneof_case_)*/{}
  };
  clear_has_data();
}

Response::~Response() {
  // @@protoc_insertion_point(destructor:Message.Response)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void Response::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  if (has_data()) {
    clear_data();
  }
}

void Response::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void Response::clear_data() {
// @@protoc_insertion_point(one_of_clear_start:Message.Response)
  switch (data_case()) {
    case kId3V2Tag: {
      if (GetArenaForAllocation() == nullptr) {
        delete _impl_.data_.id3v2tag_;
      }
      break;
    }
    case kResponse: {
      // No need to clear
      break;
    }
    case DATA_NOT_SET: {
      break;
    }
  }
  _impl_._oneof_case_[0] = DATA_NOT_SET;
}


void Response::Clear() {
// @@protoc_insertion_point(message_clear_start:Message.Response)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  clear_data();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Response::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // .Message.ID3v2Tag id3v2Tag = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          ptr = ctx->ParseMessage(_internal_mutable_id3v2tag(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .Message.Response.ResponseType response = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
          uint64_t val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_response(static_cast<::Message::Response_ResponseType>(val));
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* Response::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:Message.Response)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // .Message.ID3v2Tag id3v2Tag = 1;
  if (_internal_has_id3v2tag()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(1, _Internal::id3v2tag(this),
        _Internal::id3v2tag(this).GetCachedSize(), target, stream);
  }

  // .Message.Response.ResponseType response = 2;
  if (_internal_has_response()) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteEnumToArray(
      2, this->_internal_response(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Message.Response)
  return target;
}

size_t Response::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Message.Response)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  switch (data_case()) {
    // .Message.ID3v2Tag id3v2Tag = 1;
    case kId3V2Tag: {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
          *_impl_.data_.id3v2tag_);
      break;
    }
    // .Message.Response.ResponseType response = 2;
    case kResponse: {
      total_size += 1 +
        ::_pbi::WireFormatLite::EnumSize(this->_internal_response());
      break;
    }
    case DATA_NOT_SET: {
      break;
    }
  }
  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData Response::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    Response::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*Response::GetClassData() const { return &_class_data_; }


void Response::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<Response*>(&to_msg);
  auto& from = static_cast<const Response&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:Message.Response)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  switch (from.data_case()) {
    case kId3V2Tag: {
      _this->_internal_mutable_id3v2tag()->::Message::ID3v2Tag::MergeFrom(
          from._internal_id3v2tag());
      break;
    }
    case kResponse: {
      _this->_internal_set_response(from._internal_response());
      break;
    }
    case DATA_NOT_SET: {
      break;
    }
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void Response::CopyFrom(const Response& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Message.Response)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Response::IsInitialized() const {
  return true;
}

void Response::InternalSwap(Response* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_.data_, other->_impl_.data_);
  swap(_impl_._oneof_case_[0], other->_impl_._oneof_case_[0]);
}

::PROTOBUF_NAMESPACE_ID::Metadata Response::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_Message_2eproto_getter, &descriptor_table_Message_2eproto_once,
      file_level_metadata_Message_2eproto[2]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace Message
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::Message::Request*
Arena::CreateMaybeMessage< ::Message::Request >(Arena* arena) {
  return Arena::CreateMessageInternal< ::Message::Request >(arena);
}
template<> PROTOBUF_NOINLINE ::Message::ID3v2Tag*
Arena::CreateMaybeMessage< ::Message::ID3v2Tag >(Arena* arena) {
  return Arena::CreateMessageInternal< ::Message::ID3v2Tag >(arena);
}
template<> PROTOBUF_NOINLINE ::Message::Response*
Arena::CreateMaybeMessage< ::Message::Response >(Arena* arena) {
  return Arena::CreateMessageInternal< ::Message::Response >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
