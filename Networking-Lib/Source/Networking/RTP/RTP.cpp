#include "RTP.h"
namespace Networking {
std::vector<char> RTP::encodeToRTP(char* packet_data, size_t len) {
  return std::vector<char>(packet_data, packet_data + len);
}
std::string RTP::decodeFromRTP(std::string encodedData) { return encodedData; }
}  // namespace Networking