#pragma once
#include <array>
#include <string>
#include <vector>

namespace Networking {
class RTP {
 public:
  static std::vector<char> encodeToRTP(char* packet_data, size_t len);
  static std::vector<char> decodeFromRTP(char* encoded_packet_data, size_t len);
};
}  // namespace Networking