#pragma once
#include <cstdint>
#include <iostream>
#include <string>

#include "MP3Internals.hh"
struct ADUFrame {
  std::string header{};
  MP3SideInfo sideInfo{};
  std::string audioData{};
};
namespace Segment {
class Segment {
 public:
  Segment() {}
  // the 4-byte MPEG header
  std::string m_header{};
  // a constant (== 4)
  uint8_t m_headerSize{4};
  // the 'side info' structure, *including* the optional
  // 2 - byte CRC field, if present
  MP3SideInfo m_sideInfo{};
  // the size (in bytes) of the above structure
  uint8_t m_sideInfoSize{};
  // Remaining Frame Data
  std::string m_frameData{};
  // the size (in bytes) of the above data
  uint64_t m_frameDataSize{};
  // Value of Backpointer in bytes
  uint64_t m_backPointer{};
  // the size (in bytes) of the ADU associated with
  // this frame.(If the frame is already an "ADU Frame",
  // then aduDataSize == frameDataSize)
  uint64_t m_aduDataSize{};
  // the total size(in bytes) that this frame would have if it were a regular
  // "MP3 Frame".(If it is already a "MP3 Frame",then mp3FrameSize ==
  // headerSize + sideInfoSize + frameDataSize) Note that this size can be
  // derived completely from "header".
  uint64_t m_mp3FrameSize{};
};

class SegmentEl {
  using uint = unsigned int;

 public:
  SegmentEl() = default;
  SegmentEl(std::string frame) {
    uint hdr;
    uint frameSize;
    MP3SideInfo sideInfo;
    uint sideInfoSize;
    uint backpointer;
    uint aduSize;
    GetADUInfoFromMP3Frame(
        reinterpret_cast<const unsigned char *>(frame.c_str()), frame.length(),
        hdr, frameSize, sideInfo, sideInfoSize, backpointer, aduSize);
    m_segment.m_header = frame.substr(0, 4);
    m_segment.m_headerSize = 4;
    m_segment.m_sideInfo = sideInfo;
    m_segment.m_sideInfoSize = sideInfoSize;
    auto frameDataStart = m_segment.m_headerSize + sideInfoSize;
    auto frameDataEnd = m_segment.m_headerSize + sideInfoSize + aduSize;
    auto count = frameDataEnd - frameDataStart;
    m_segment.m_frameData = frame.substr(frameDataStart, count);
    m_segment.m_frameDataSize = m_segment.m_frameData.size();
    m_segment.m_aduDataSize = aduSize;
    m_segment.m_backPointer = backpointer;
    m_segment.m_mp3FrameSize = frameSize;
  }

  SegmentEl(ADUFrame aduFrame) {
    m_segment.m_header = aduFrame.header;
    m_segment.m_headerSize = 4;
    m_segment.m_sideInfo = aduFrame.sideInfo;
    m_segment.m_sideInfoSize = 32;
    m_segment.m_frameData = aduFrame.audioData;
    m_segment.m_frameDataSize = aduFrame.audioData.size();
    unsigned numBits = m_segment.m_sideInfo.ch[0].gr[0].part2_3_length;
    numBits += m_segment.m_sideInfo.ch[0].gr[1].part2_3_length;
    numBits += m_segment.m_sideInfo.ch[1].gr[0].part2_3_length;
    numBits += m_segment.m_sideInfo.ch[1].gr[1].part2_3_length;
    auto aduSize = (numBits + 7) / 8;
    m_segment.m_aduDataSize = aduSize;
    m_segment.m_backPointer = m_segment.m_sideInfo.main_data_begin;
    MP3FrameParams fr;
    const unsigned char *header =
        reinterpret_cast<const unsigned char *>(aduFrame.header.c_str());
    fr.hdr = ((unsigned)header[0] << 24) | ((unsigned)header[1] << 16) |
             ((unsigned)header[2] << 8) | (unsigned)header[3];
    fr.setParamsFromHeader();
    auto frameSize = 4 + fr.frameSize;
    m_segment.m_mp3FrameSize = frameSize;
  }
  Segment m_segment{};
  SegmentEl *m_next{};
  SegmentEl *m_prev{};

 private:
};

class SegmentQueue {
 public:
  SegmentQueue() {}
  void enqueue(SegmentEl *seg);
  SegmentEl *dequeue();
  bool isEmpty();
  SegmentEl *head();
  SegmentEl *tail();
  // returns the segment prior to a
  // given one
  SegmentEl *previous(SegmentEl *seg);
  // returns the segment after a given one
  SegmentEl *next(SegmentEl *seg);
  // returns the sum of the "frameDataSize" fields
  // of each entry in the queue
  uint64_t totalDataSize();

 private:
  SegmentEl *m_head{};
  SegmentEl *m_tail{};
};
}  // namespace Segment