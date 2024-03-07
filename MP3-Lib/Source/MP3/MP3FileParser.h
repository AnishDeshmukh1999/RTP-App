#include <boost/filesystem.hpp>
#include <fstream>
#include <string>

#include "ID3v2TagHeader.pb.h"
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
    // file.open(m_filename);
  }
  ~MP3FileParser() { file.close(); }
  std::unique_ptr<ID3v2::ID3v2Tag> getTag();

 private:
  std::string m_filename{};
  std::ifstream file;
  int m_num_frames{};
};
}  // namespace FileParser