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

uint32_t unsignedCharToUint32(char bytes[4]) {
  return (uint32_t)(bytes[3]) | (uint32_t)(bytes[2]) << 8 |
         (uint32_t)(bytes[1]) << 16 | (uint32_t)(bytes[0]) << 24;
}

Message::SongInfo MP3FileParser::findInfo() {
  Message::SongInfo tag;
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
  tag.set_tagsize(uint_tagsize);

  m_id3FrameSize = uint_tagsize;

  file.seekg(0, file.beg);

  // Read and parse Entire Tag
  unsigned char* data = new unsigned char[uint_tagsize];
  file.read((char*)(data), uint_tagsize);
  if (file.fail()) {
    return tag;
  }
  std::string str_data(reinterpret_cast<char*>(data), uint_tagsize);
  tag.set_id3tag_data(str_data);

  // Set NumFrames after
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

std::unique_ptr<Message::SongInfo> MP3FileParser::getInfo() {
  return std::make_unique<Message::SongInfo>(m_SongInfo);
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

// <Sampling rate, MPEG Version>
std::map<std::pair<int, int>, int> sampling_rate_freq_idx_map = {
    // MPEG 1
    {{0b00, 0b11}, 44100},
    {{0b01, 0b11}, 48000},
    {{0b10, 0b11}, 32000},
    {{0b11, 0b11}, -1},
    // MPEG 2
    {{0b00, 0b10}, 22050},
    {{0b01, 0b10}, 24000},
    {{0b10, 0b10}, 16000},
    {{0b11, 0b10}, -1},
    // MPEG 2.5
    {{0b00, 0b00}, 11025},
    {{0b01, 0b00}, 12000},
    {{0b10, 0b00}, 8000},
    {{0b11, 0b00}, -1},
};

// <Layer, MPEG Version>
std::map<std::pair<uint8_t, uint8_t>, uint64_t> samples_per_frame_map = {
    // MPEG 1
    // Layer III
    {{0b01, 0b11}, 1152},
    // Layer II
    {{0b10, 0b11}, 1152},
    // Layer I
    {{0b11, 0b11}, 384},
    // MPEG 2
    // Layer III
    {{0b01, 0b10}, 576},
    // Layer II
    {{0b10, 0b10}, 1152},
    // Layer I
    {{0b11, 0b10}, 384},
    // MPEG 2.5
    // Layer III
    {{0b01, 0b00}, 576},
    // Layer II
    {{0b10, 0b00}, 1152},
    // Layer I
    {{0b11, 0b00}, 384},
};

// Get MP3 frame from m_frameOffset
std::string MP3FileParser::getMP3FrameFromOffset(uint8_t& mpegVersion,
                                                 uint8_t& channelMode,
                                                 uint8_t& samplingRateIdx,
                                                 uint8_t& layer) {
  file.seekg(m_fileOffset, file.beg);
  unsigned char frameHeader[4]{};
  file.read((char*)(&frameHeader[0]), 4);
  if (file.fail()) {
    return "";
  }

  //// Check for frame sync
  // if ((static_cast<int>(frameHeader[0]) & 0b11111111) != 0b11111111 &&
  //     ((static_cast<int>(frameHeader[1]) >> 5) & 0b111) != 0b111) {
  //   std::cout << "Not an MP3 Header \n";
  //   return "";
  // }

  if (!isFrameSync(reinterpret_cast<char*>(frameHeader))) {
    std::cout << "Not an MP3 Header \n";
    return "";
  }

  uint8_t mpegLayer = (static_cast<int>(frameHeader[1]) >> 1) & 0b11;
  if (mpegLayer == 0b11) {
    std::cout << "Deal with MPEG Layer I \n";
    return "";
  }

  // Get MPEG Version
  mpegVersion = (static_cast<int>(frameHeader[1]) >> 3) & 0b11;

  // Get Channel Mode
  channelMode = (static_cast<int>(frameHeader[3]) >> 6) & 0b11;

  // Get Sampling Rate Index
  samplingRateIdx = (static_cast<int>(frameHeader[2]) >> 2) & 0b11;

  // Get Layer
  layer = (static_cast<int>(frameHeader[1]) >> 1) & 0b11;
  uint64_t bitrateIdxKey = (static_cast<int>(frameHeader[2]) >> 4) & 0b1111;
  uint8_t padding_bit = (static_cast<int>(frameHeader[2]) >> 1) & 0b1;
  uint64_t frameSize = (144 * mp3_bitrate_v1_l3[bitrateIdxKey]) /
                           (mpeg1_sampling_rates[samplingRateIdx]) +
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

MP3::Song MP3FileParser::getAllFramesAndCalculateDetails(
    MP3FileParser& fileParser) {
  std::string curRes;
  uint8_t mpegVersion;
  uint8_t channelMode;
  uint8_t samplingRateIdx;
  uint8_t layer;
  // Sanity Check counter
  long numFrames = 0;
  long temp = fileParser.getSize();
  long numBytes = temp - fileParser.m_id3FrameSize;
  long curBytesIdx = 0;
  fileParser.m_fileOffset = fileParser.m_id3FrameSize;
  while (1) {
    curRes = fileParser.getMP3FrameFromOffset(mpegVersion, channelMode,
                                              samplingRateIdx, layer);
    if (curRes.length() == 0) {
      break;
    }
    fileParser.m_songFrames.push_back(curRes);
    curBytesIdx += curRes.length();
    numFrames += 1;
    if (numFrames == 7966) {
      int k = 10;
    }
  }
  int samplesPerFrame =
      samples_per_frame_map[std::make_pair(layer, mpegVersion)];
  int samplingRate =
      sampling_rate_freq_idx_map[std::make_pair(samplingRateIdx, mpegVersion)];
  int duration = numFrames * samplesPerFrame / samplingRate;
  MP3::Song song;
  song.m_numFrames = numFrames;
  song.m_DurationSeconds = duration;
  return song;
}

MP3::Song MP3FileParser::getSongDetails() { return m_song; }
}  // namespace FileParser