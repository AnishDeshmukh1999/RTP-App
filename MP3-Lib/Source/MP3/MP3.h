#pragma once
#define TAGLIB_STATIC
#include <stdio.h>
#include <taglib/fileref.h>
#include <taglib/mpeg/id3v2/frames/attachedpictureframe.h>
#include <taglib/mpeg/id3v2/id3v2frame.h>
#include <taglib/mpeg/id3v2/id3v2framefactory.h>
#include <taglib/mpeg/id3v2/id3v2tag.h>
#include <taglib/mpeg/mpegfile.h>
#include <taglib/mpeg/mpegproperties.h>
#include <tbytevectorstream.h>
#include <tfile.h>

#include <cmath>
#include <filesystem>
#include <string>

#include "Networking/Message.pb.h"

namespace MP3 {
struct Tag {
  std::string title{"   "};
  std::string artist{"   "};
  std::string album{"   "};
  int year{1800};
};
struct Song {
  uint64_t m_numFrames{};
  uint64_t m_FirstFrameByteOffset{};
};
class MP3 {
 private:
  static const std::string stringToHexString(const std::string& input) {
    std::stringstream stream;

    // Output each character of the input string as a hexadecimal representation
    for (char c : input) {
      stream << std::hex << std::setw(2) << std::setfill('0')
             << static_cast<int>(c);
    }
    return stream.str();
  }

 public:
  // Get size of ID3 Frame including the Required and extension/optional
  // header
  static uint64_t getID3FrameSize(const Message::ID3v2Tag* tag) {
    const std::string data = tag->data();
    std::string s_identifier = data.substr(0, 3);
    if (s_identifier != "ID3") {
      return 0;
    }
    std::string s_size = data.substr(6, 4);
    int powerArr[4] = {21, 14, 7, 0};
    uint64_t res = 0;
    int idx = 0;
    for (auto el : s_size) {
      res += static_cast<int>(el) *
             static_cast<uint64_t>(std::pow(2, powerArr[idx++]));
    }
    // ID3 size field doesn't include the ID3 main header size so we add 10
    // bytes to the result
    return res + 10;
  }
  static Tag getTagFields(const Message::ID3v2Tag& tag) {
    const std::string tag_data = tag.data();
    const auto data = TagLib::ByteVector(tag_data.c_str(), tag_data.length());
    auto stream = TagLib::ByteVectorStream(data);
    auto file =
        TagLib::MPEG::File(&stream, TagLib::ID3v2::FrameFactory::instance());
    TagLib::Tag* parsed_tag = file.tag();
    Tag result;
    result.title = parsed_tag->title().toCString();
    result.artist = parsed_tag->artist().toCString();
    result.album = parsed_tag->album().toCString();
    result.year = parsed_tag->year();
    return result;
  }

  static std::string getImageDataAsString(const Message::ID3v2Tag& tag) {
    std::string tag_data = tag.data();
    const auto data = TagLib::ByteVector(tag_data.c_str(), tag_data.length());
    auto stream = TagLib::ByteVectorStream(data);
    auto file =
        TagLib::MPEG::File(&stream, TagLib::ID3v2::FrameFactory::instance());
    TagLib::Tag* parsed_tag = file.tag();
    TagLib::StringList names = file.complexPropertyKeys();
    for (const auto& name : names) {
      const auto& properties = file.complexProperties(name);
      for (const auto& property : properties) {
        for (const auto& [key, value] : property) {
          if (value.type() == TagLib::Variant::ByteVector) {
            const std::string res(value.value<TagLib::ByteVector>().data(),
                                  value.value<TagLib::ByteVector>().size());
            return res;
          }
        }
      }
    }
    return "";
  }
};
}  // namespace MP3