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
  }
  ~MP3FileParser() { file.close(); }
  std::unique_ptr<Message::SongInfo> getInfo();
  std::string getMP3FrameFromOffset();
  MP3::Song getSongDetails();

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