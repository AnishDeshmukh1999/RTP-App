#pragma once
#include <array>
#include <string>
#include <vector>

namespace Networking {
class RTP {
 public:
  static std::vector<char> encodeToRTP(char* packet_data, size_t len);
  static std::string decodeFromRTP(std::string encodedData);
};
}  // namespace Networking