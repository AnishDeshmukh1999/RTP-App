#define TAGLIB_STATIC
#include <stdio.h>
#include <taglib/fileref.h>
#include <taglib/mpeg/id3v2/id3v2framefactory.h>
#include <taglib/mpeg/mpegfile.h>
#include <taglib/mpeg/mpegproperties.h>
#include <tbytevectorstream.h>
#include <tfile.h>

#include <filesystem>
#include <string>

#include "Networking/Message.pb.h"

namespace MP3 {
struct Tag {
  std::string title{"???"};
  std::string artist{"???"};
  std::string album{"???"};
  int year{1800};
};
class MP3 {
 public:
  static Tag getTagFields(Message::ID3v2Tag& tag) {
    std::string tag_data = tag.data();
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
};
}  // namespace MP3