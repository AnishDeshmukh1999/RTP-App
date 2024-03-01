#include "RTP.h"
namespace Networking {
std::vector<char> RTP::encodeToRTP(char* packet_data, size_t len) {
  return std::vector<char>(packet_data, packet_data + len);
}
std::vector<char> RTP::decodeFromRTP(char* encoded_packet_data, size_t len) {
  return std::vector<char>(encoded_packet_data, encoded_packet_data + len);
}
}  // namespace Networking