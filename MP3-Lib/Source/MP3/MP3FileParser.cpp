#include "MP3FileParser.h"
namespace FileParser {
bool validateIdentifier(unsigned char bytes[3]) {
  if (bytes[0] == '\x49' && bytes[1] == '\x44' && bytes[2] == '\x33') {
    return true;
  }
  return false;
}

bool validateTagSize(unsigned char bytes[4]) {
  auto a1 = bytes[0];
  auto a2 = bytes[1];
  auto a3 = bytes[2];
  auto a4 = bytes[3];
  // if (bytes[0] >= '\x80') {
  //   int k = 10;
  // }
  // if (bytes[1] >= '\x80') {
  //   int k = 10;
  // }
  // if (bytes[2] >= '\x80') {
  //   int k = 10;
  // }
  // if (bytes[3] >= '\x80') {
  //   int k = 10;
  // }
  if (bytes[0] >= 0x80 || bytes[1] >= 0x80 || bytes[2] >= 0x80 ||
      bytes[3] >= 0x80) {
    return false;
  }
  return true;
}

uint32_t unsignedCharToUint32(unsigned char bytes[4]) {
  return (uint32_t)(bytes[3]) | (uint32_t)(bytes[2]) << 8 |
         (uint32_t)(bytes[1]) << 16 | (uint32_t)(bytes[0]) << 24;
}

std::unique_ptr<ID3v2::ID3v2Tag> MP3FileParser::getTag() {
  std::unique_ptr<ID3v2::ID3v2Tag> tag_ptr;
  ID3v2::ID3v2Tag tag;
  file.seekg(0, file.beg);

  // Read and parse identifier
  unsigned char identifier[4] = "???";
  file.read((char*)(&identifier[0]), 3);
  if (file.fail()) {
    return std::unique_ptr<ID3v2::ID3v2Tag>(nullptr);
  }
  if (!validateIdentifier(identifier)) {
    return std::unique_ptr<ID3v2::ID3v2Tag>(nullptr);
  }
  std::string bytes(reinterpret_cast<char*>(identifier), sizeof(identifier));
  tag.set_identifier(bytes);

  // Read and parse major version
  unsigned char majorversion = '?';
  file.read((char*)(&majorversion), 1);
  if (file.fail()) {
    return std::unique_ptr<ID3v2::ID3v2Tag>(nullptr);
  }
  uint32_t uint_majorversion = majorversion;
  tag.set_majorversion(uint_majorversion);

  // Read and parse Minor Version
  unsigned char minorversion = '?';
  file.read((char*)(&minorversion), 1);
  if (file.fail()) {
    return std::unique_ptr<ID3v2::ID3v2Tag>(nullptr);
  }
  uint32_t uint_minorversion = minorversion;
  tag.set_minorversion(uint_minorversion);

  // Read and parse Flags
  unsigned char flags = '?';
  file.read((char*)(&flags), 1);
  if (file.fail()) {
    return std::unique_ptr<ID3v2::ID3v2Tag>(nullptr);
  }
  uint32_t uint_flags = flags;
  tag.set_flags(uint_flags);

  // Read and parse Size
  unsigned char tagsize[5] = "????";
  file.read((char*)(&tagsize[0]), 4);
  if (file.fail()) {
    return std::unique_ptr<ID3v2::ID3v2Tag>(nullptr);
  }
  if (!validateTagSize(tagsize)) {
    return std::unique_ptr<ID3v2::ID3v2Tag>(nullptr);
  }
  uint32_t uint_tagsize = unsignedCharToUint32(tagsize);
  tag.set_s_tagsize(uint_tagsize);

  file.seekg(0, file.beg);

  // Read and parse Entire Tag
  unsigned char* data = new unsigned char[uint_tagsize];
  file.read((char*)(data), uint_tagsize);
  if (file.fail()) {
    return std::unique_ptr<ID3v2::ID3v2Tag>(nullptr);
  }
  std::string str_data(reinterpret_cast<char*>(data));
  tag.set_data(str_data);
  delete[] data;
  return std::unique_ptr<ID3v2::ID3v2Tag>(new ID3v2::ID3v2Tag(std::move(tag)));
}
}  // namespace FileParser