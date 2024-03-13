#pragma once
#include <MP3/Segment.h>

#include <cstdint>
#include <functional>
#include <string>
namespace ADU {
using namespace Segment;
struct QueueInfo {
  SegmentQueue pendingMP3Frames;
  uint64_t currentFrameNum{};
  uint64_t numFrames{};
  uint64_t totalDataSizeBefore{0};
  uint64_t totalDataSizeAfter{0};
};
class ADU {
  using uint = unsigned int;
  struct ADUFrame {
    std::string header{};
    MP3SideInfo sideInfo{};
    std::string audioData{};
  };

 public:
  static std::vector<ADUFrame> MP3ToADU(std::vector<std::string>& frames,
                                        QueueInfo& queueInfo) {
    std::string res;
    ADUFrame aduFrame;
    SegmentEl* newFrame{};
    uint idx{0};
    std::vector<ADUFrame> ADUFrames;
    while (1) {
      if (idx == frames.size()) {
        return ADUFrames;
      }
      if (idx == 12) {
        int k = 10;
      }
      // Enqueue new MP3 Frames, until we have enough data to
      // generate the ADU for a frame:
      do {
        queueInfo.totalDataSizeBefore =
            queueInfo.pendingMP3Frames.totalDataSize();
        if (idx == frames.size()) {
          return ADUFrames;
        }

        newFrame = new SegmentEl(frames[idx++]);
        queueInfo.pendingMP3Frames.enqueue(newFrame);
        queueInfo.currentFrameNum += 1;
        queueInfo.totalDataSizeAfter =
            queueInfo.pendingMP3Frames.totalDataSize();
      } while (
          queueInfo.totalDataSizeBefore < newFrame->m_segment.m_backPointer ||
          queueInfo.totalDataSizeAfter < newFrame->m_segment.m_aduDataSize);

      // We now have enough data to generate the ADU for the most
      // recently enqueued frame (i.e., the tail of the queue).
      // (The earlier frames in the queue -- if any -- must be
      // discarded, as we don't have enough data to generate
      // their ADUs.)
      SegmentEl* tailFrame = queueInfo.pendingMP3Frames.tail();
      aduFrame.header = tailFrame->m_segment.m_header;
      aduFrame.sideInfo = tailFrame->m_segment.m_sideInfo;
      aduFrame.audioData = "";
      // Go back to the frame that contains the start of our
      // ADU data:
      uint64_t offset = 0;
      SegmentEl* curFrame = tailFrame;
      uint64_t prevBytes = tailFrame->m_segment.m_backPointer;
      while (prevBytes > 0) {
        curFrame = queueInfo.pendingMP3Frames.previous(curFrame);
        uint64_t dataHere = curFrame->m_segment.m_frameDataSize;
        if (dataHere < prevBytes) {
          prevBytes -= dataHere;
        } else {
          offset = dataHere - prevBytes;
          break;
        }
      }
      // Dequeue any frames that we no longer need:
      while (queueInfo.pendingMP3Frames.head() != curFrame) {
        queueInfo.pendingMP3Frames.dequeue();
      }
      // Output, from the remaining frames, the ADU data that
      // we want:
      uint64_t bytesToUse = tailFrame->m_segment.m_aduDataSize;
      while (bytesToUse > 0) {
        uint64_t dataHere = curFrame->m_segment.m_frameDataSize - offset;
        uint64_t bytesUsedHere = dataHere < bytesToUse ? dataHere : bytesToUse;

        // output("bytesUsedHere" bytes from curFrame.frameData,
        //       starting from "offset");
        aduFrame.audioData += std::string(
            (char*)(&curFrame->m_segment.m_frameData[offset]), bytesUsedHere);

        ADUFrames.push_back(aduFrame);

        bytesToUse -= bytesUsedHere;
        offset = 0;
        curFrame = queueInfo.pendingMP3Frames.next(curFrame);
      }
    }
  }
};
}  // namespace ADU