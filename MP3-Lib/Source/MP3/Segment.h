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
class ADUFrameEl {
 public:
  ADUFrame m_frame{};
  ADUFrameEl *m_next{};
  ADUFrameEl *m_prev{};
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
  SegmentEl(Segment segment) { m_segment = segment; }

  SegmentEl(ADUFrameEl aduFrameEl) {
    m_segment.m_header = aduFrameEl.m_frame.header;
    m_segment.m_headerSize = 4;
    m_segment.m_sideInfo = aduFrameEl.m_frame.sideInfo;
    m_segment.m_sideInfoSize = 32;
    m_segment.m_frameData = aduFrameEl.m_frame.audioData;
    m_segment.m_frameDataSize = aduFrameEl.m_frame.audioData.size();
    unsigned numBits = m_segment.m_sideInfo.ch[0].gr[0].part2_3_length;
    numBits += m_segment.m_sideInfo.ch[0].gr[1].part2_3_length;
    numBits += m_segment.m_sideInfo.ch[1].gr[0].part2_3_length;
    numBits += m_segment.m_sideInfo.ch[1].gr[1].part2_3_length;
    auto aduSize = (numBits + 7) / 8;
    m_segment.m_aduDataSize = aduSize;
    m_segment.m_backPointer = m_segment.m_sideInfo.main_data_begin;
    MP3FrameParams fr;
    const unsigned char *header = reinterpret_cast<const unsigned char *>(
        aduFrameEl.m_frame.header.c_str());
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

template <typename T>
concept QueueElement = requires(T v) {
  v.m_next;
  v.m_prev;
};

template <QueueElement T>
class SegmentQueue {
 public:
  SegmentQueue() {}
  void enqueue(T *seg);
  T *dequeue();
  bool isEmpty();
  T *head();
  T *tail();
  // returns the segment prior to a
  // given one
  T *previous(T *seg);
  // returns the segment after a given one
  T *next(T *seg);
  // returns the sum of the "frameDataSize" fields
  // of each entry in the queue
  uint64_t totalDataSize();
  const int getNumElements() { return m_numElements; }

 private:
  T *m_head{};
  T *m_tail{};
  int m_numElements{};
};

}  // namespace Segment

template <Segment::QueueElement T>
void Segment::SegmentQueue<T>::enqueue(T *seg) {
  m_numElements += 1;
  if (!m_head) {
    m_head = seg;
    m_tail = seg;
  } else {
    T *prevTail = m_tail;
    seg->m_next = prevTail;
    prevTail->m_prev = seg;
    m_tail = seg;
  }
}

template <Segment::QueueElement T>
T *Segment::SegmentQueue<T>::dequeue() {
  if (!m_head) {
    return nullptr;
  }
  if (m_head == m_tail) {
    T *temp = m_head;
    m_numElements -= 1;
    m_head = nullptr;
    m_tail = nullptr;
    return temp;
  }
  T *res = m_head;
  T *newHead = m_head->m_prev;
  m_head->m_prev = nullptr;
  newHead->m_next = nullptr;
  m_head = newHead;
  m_numElements -= 1;
  return res;
}

template <Segment::QueueElement T>
bool Segment::SegmentQueue<T>::isEmpty() {
  return (m_head == nullptr) ? true : false;
}

template <Segment::QueueElement T>
T *Segment::SegmentQueue<T>::head() {
  return m_head;
}

template <Segment::QueueElement T>
T *Segment::SegmentQueue<T>::tail() {
  return m_tail;
}

template <Segment::QueueElement T>
T *Segment::SegmentQueue<T>::previous(T *seg) {
  return seg->m_prev;
}

template <Segment::QueueElement T>
T *Segment::SegmentQueue<T>::next(T *seg) {
  return seg->m_next;
}

template <Segment::QueueElement T>
uint64_t Segment::SegmentQueue<T>::totalDataSize() {
  T *node = m_head;
  uint64_t res = 0;
  while (node != nullptr) {
    res += node->m_segment.m_frameDataSize;
    node = node->m_prev;
  }
  return res;
}