#pragma once
#include <boost/filesystem.hpp>
#include <fstream>
#include <string>

#include "../../../Networking-Lib/Source/Networking/Message.pb.h"
#include "MP3/MP3.h"
namespace FileParser {
class Util {
 public:
  static bool checkForValidFileName(std::string file_name) {
    return boost::filesystem::exists(file_name);
  }
};
class MP3FileParser {
 public:
  // Assumes that the file_name exists and is valid. Check using
  MP3FileParser(std::string file_name)
      : m_filename{file_name},
        file(file_name, std::ios::binary | std::ios::ate) {
    m_SongInfo = findInfo();
    m_fileOffset = m_id3FrameSize;
    m_song = MP3FileParser::getAllFramesAndCalculateDetails(*this);
  }
  ~MP3FileParser() { file.close(); }
  std::unique_ptr<Message::SongInfo> getInfo();
  std::string getMP3FrameFromOffset(uint8_t& mpegVersion, uint8_t& channelMode,
                                    uint8_t& samplingRateIdx, uint8_t& layer);
  MP3::Song getSongDetails();
  static MP3::Song getAllFramesAndCalculateDetails(MP3FileParser& fileParser);
  long getSize() {
    long begin, end;
    file.seekg(0, file.beg);
    begin = file.tellg();
    file.seekg(0, file.end);
    end = file.tellg();
    return end - begin;
  }

  MP3::Song m_song{};
  std::vector<std::string> m_songFrames{};

 private:
  Message::SongInfo findInfo();
  std::string m_filename{};
  std::ifstream file;
  int m_num_frames{};
  Message::SongInfo m_SongInfo;
  int m_fileOffset{};
  uint64_t m_id3FrameSize{};
  bool isFrameSync(const std::string& header);
};
}  // namespace FileParser