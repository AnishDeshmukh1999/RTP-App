#pragma once
#include <bit>
#include <cstdint>
#include <iostream>
#include <vector>
namespace Networking {
struct RTPHeader {
  unsigned char version : 2;
  unsigned char padding : 1;
  unsigned char extension : 1;
  unsigned char cc : 4;  // CSRC Count
  unsigned char marker : 1;
  unsigned char payload_type : 7;
  unsigned char sequence_number : 16;
  unsigned char timestamp : 32;
  unsigned char ssrc : 32;
  std::vector<uint32_t> csrc_entries;  // Each entry is 32 bits. Number of
                                       // entries is given by cc field
  unsigned char profile_ex_header_ID : 16;
  unsigned char extension_header_length : 16;
  std::vector<uint32_t> extension_header;
};
};  // namespace Networking