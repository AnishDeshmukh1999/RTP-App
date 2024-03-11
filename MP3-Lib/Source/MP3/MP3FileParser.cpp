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

Message::ID3v2Tag MP3FileParser::findTag() {
  Message::ID3v2Tag tag;
  file.seekg(0, file.beg);

  // Read and parse identifier
  unsigned char identifier[4] = "???";
  file.read((char*)(&identifier[0]), 3);
  if (file.fail()) {
    return tag;
  }
  if (!validateIdentifier(identifier)) {
    return tag;
  }
  std::string bytes(reinterpret_cast<char*>(identifier), sizeof(identifier));
  tag.set_identifier(bytes);

  // Read and parse major version
  unsigned char majorversion = '?';
  file.read((char*)(&majorversion), 1);
  if (file.fail()) {
    return tag;
  }
  uint32_t uint_majorversion = majorversion;
  tag.set_majorversion(uint_majorversion);

  // Read and parse Minor Version
  unsigned char minorversion = '?';
  file.read((char*)(&minorversion), 1);
  if (file.fail()) {
    return tag;
  }
  uint32_t uint_minorversion = minorversion;
  tag.set_minorversion(uint_minorversion);

  // Read and parse Flags
  unsigned char flags = '?';
  file.read((char*)(&flags), 1);
  if (file.fail()) {
    return tag;
  }
  uint32_t uint_flags = flags;
  tag.set_flags(uint_flags);

  // Read and parse Size
  unsigned char tagsize[5] = "????";
  file.read((char*)(&tagsize[0]), 4);
  if (file.fail()) {
    return tag;
  }
  if (!validateTagSize(tagsize)) {
    return tag;
  }
  uint32_t uint_tagsize = MP3::MP3::getID3FrameSize(
      std::string(tagsize, tagsize + sizeof tagsize / sizeof tagsize[0]));
  tag.set_s_tagsize(uint_tagsize);

  m_id3FrameSize = uint_tagsize;

  file.seekg(0, file.beg);

  // Read and parse Entire Tag
  unsigned char* data = new unsigned char[uint_tagsize];
  file.read((char*)(data), uint_tagsize);
  if (file.fail()) {
    return tag;
  }
  std::string str_data(reinterpret_cast<char*>(data), uint_tagsize);
  tag.set_data(str_data);
  return tag;
}

bool MP3FileParser::isFrameSync(const std::string& header) {
  if (header.length() < 2) {
    return false;
  }
  int frameSync[2]{0b11111111, 0b11100000};
  if (!((static_cast<int>(header[0]) & frameSync[0]) == frameSync[0] &&
        (static_cast<int>(header[1]) & frameSync[1]) == frameSync[1])) {
    return false;
  }
  return true;
}

std::unique_ptr<Message::ID3v2Tag> MP3FileParser::getTag() {
  return std::make_unique<Message::ID3v2Tag>(m_tag);
}

std::map<int, int> mp3_bitrate_v1_l3 = {
    {0b0000, -1},  {0b0001, 32},  {0b0010, 40},  {0b0011, 48},
    {0b0100, 56},  {0b0101, 64},  {0b0110, 80},  {0b0111, 96},
    {0b1000, 112}, {0b1001, 128}, {0b1010, 160}, {0b1011, 192},
    {0b1100, 224}, {0b1101, 256}, {0b1110, 320}, {0b1111, -1}};

/*
mpeg1_sampling_rates = {
    '00': 44100,
    '01': 48000,
    '10': 32000,
    '11': 'reserv.'
}
*/
std::map<int, float> mpeg1_sampling_rates = {
    {0, 44.1f}, {1, 48.0f}, {2, 32.0f}};

// <MPEG Version,channel mode>
std::map<std::pair<uint8_t, uint8_t>, uint64_t> xing_header_offset_map = {
    // MPEG 1
    {{0b11, 0b00}, 32},
    {{0b11, 0b01}, 32},
    {{0b11, 0b10}, 32},
    // MPEG 1 Mono
    {{0b11, 0b11}, 17},
    // MPEG 2
    {{0b10, 0b00}, 17},
    {{0b10, 0b01}, 17},
    {{0b10, 0b10}, 17},
    // MPEG 2 Mono
    {{0b10, 0b11}, 9},
    // MPEG 2.5
    {{0b00, 0b00}, 17},
    {{0b00, 0b01}, 17},
    {{0b00, 0b10}, 17},
    // MPEG 2 Mono
    {{0b00, 0b11}, 9}};

// Get MP3 frame from m_frameOffset
std::string MP3FileParser::getMP3FrameFromOffset() {
  file.seekg(m_fileOffset, file.beg);
  unsigned char frameHeader[4]{};
  file.read((char*)(&frameHeader[0]), 4);
  if (file.fail()) {
    return "";
  }
  uint8_t mpegLayer = (static_cast<int>(frameHeader[1]) >> 1) & 0b11;
  if (mpegLayer == 0b11) {
    std::cout << "Deal with MPEG Layer I \n";
    return "";
  }

  uint64_t bitrateIdxKey = (static_cast<int>(frameHeader[2]) >> 4) & 0b1111;
  uint64_t samplingRateKey = (static_cast<int>(frameHeader[2]) >> 2) & 0b11;
  uint8_t padding_bit = (static_cast<int>(frameHeader[2]) >> 1) & 0b1;
  uint64_t frameSize = (144 * mp3_bitrate_v1_l3[bitrateIdxKey]) /
                           (mpeg1_sampling_rates[samplingRateKey]) +
                       padding_bit;
  file.seekg(m_fileOffset);

  unsigned char* mp3Frame = new unsigned char[frameSize];
  file.read((char*)(&mp3Frame[0]), frameSize);
  if (file.fail()) {
    std::cout << "Failed to read MP3 frame into buffer \n";
    return "";
  }
  std::string frame(reinterpret_cast<char*>(mp3Frame), frameSize);
  m_fileOffset += frameSize;
  return frame;
}
MP3::Song MP3FileParser::getSongDetails() {
  MP3::Song song;
  file.seekg(m_id3FrameSize);

  unsigned char header[128]{};
  file.read((char*)(&header[0]), 128);
  if (file.fail()) {
    std::cout << "Failed to read from file \n";
    return song;
  }

  if (!isFrameSync(std::string(reinterpret_cast<char*>(header), 3))) {
    std::cout << "Frame Sync not found! ";
    return song;
  }
  // Get MPEG Version
  int t = (static_cast<int>(header[1]) >> 3);
  uint8_t mpegVersion = (t) & 0b11;

  // Get Channel Mode
  uint8_t channelMode = (static_cast<int>(header[3]) >> 6) & 0b11;
  // Check for XING Header
  uint64_t xingHeaderOffset =
      m_id3FrameSize + 4 +
      xing_header_offset_map[std::make_pair(mpegVersion, channelMode)];

  return MP3::Song();
}
}  // namespace FileParser