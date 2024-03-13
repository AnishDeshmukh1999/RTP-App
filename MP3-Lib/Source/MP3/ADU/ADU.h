#pragma once
#include <MP3/Segment.h>

#include <cstdint>
#include <functional>
#include <string>
namespace ADU {
using namespace Segment;
struct QueueInfo {
  SegmentQueue pendingFrames;
  uint64_t currentFrameNum{};
  uint64_t numFrames{};
  uint64_t totalDataSizeBefore{0};
  uint64_t totalDataSizeAfter{0};
};
class ADU {
  using uint = unsigned int;

 public:
  static std::vector<ADUFrame> MP3ToADU(std::vector<std::string>& frames,
                                        QueueInfo& queueInfo) {
    std::string res;
    ADUFrame aduFrame;
    SegmentEl newFrame;
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
        queueInfo.totalDataSizeBefore = queueInfo.pendingFrames.totalDataSize();
        if (idx == frames.size()) {
          return ADUFrames;
        }

        newFrame = SegmentEl(frames[idx++]);
        queueInfo.pendingFrames.enqueue(&newFrame);
        queueInfo.currentFrameNum += 1;
        queueInfo.totalDataSizeAfter = queueInfo.pendingFrames.totalDataSize();
      } while (queueInfo.totalDataSizeBefore <
                   newFrame.m_segment.m_backPointer ||
               queueInfo.totalDataSizeAfter < newFrame.m_segment.m_aduDataSize);

      // We now have enough data to generate the ADU for the most
      // recently enqueued frame (i.e., the tail of the queue).
      // (The earlier frames in the queue -- if any -- must be
      // discarded, as we don't have enough data to generate
      // their ADUs.)
      SegmentEl* tailFrame = queueInfo.pendingFrames.tail();
      aduFrame.header = tailFrame->m_segment.m_header;
      aduFrame.sideInfo = tailFrame->m_segment.m_sideInfo;
      aduFrame.audioData = "";
      // Go back to the frame that contains the start of our
      // ADU data:
      uint64_t offset = 0;
      SegmentEl* curFrame = tailFrame;
      uint64_t prevBytes = tailFrame->m_segment.m_backPointer;
      while (prevBytes > 0) {
        curFrame = queueInfo.pendingFrames.previous(curFrame);
        uint64_t dataHere = curFrame->m_segment.m_frameDataSize;
        if (dataHere < prevBytes) {
          prevBytes -= dataHere;
        } else {
          offset = dataHere - prevBytes;
          break;
        }
      }
      // Dequeue any frames that we no longer need:
      while (queueInfo.pendingFrames.head() != curFrame) {
        queueInfo.pendingFrames.dequeue();
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
        SegmentEl temp(aduFrame);

        bytesToUse -= bytesUsedHere;
        offset = 0;
        curFrame = queueInfo.pendingFrames.next(curFrame);
      }
    }
  }

  static void ADUToMP3(QueueInfo& queueInfo, std::vector<ADUFrame>& ADUFrames) {
    SegmentEl* newADU;
    int aduIdx = 0;

    while (1) {
      while (needToGetAnADU(queueInfo)) {
        newADU = new SegmentEl(ADUFrames[aduIdx++]);
        queueInfo.pendingFrames.enqueue(newADU);

        insertDummyADUsIfNecessary(queueInfo);
      }

      generateFrameFromHeadADU(queueInfo);
    }
  }

  static bool needToGetAnADU(QueueInfo& queueInfo) {
    // Checks whether we need to enqueue one or more new ADUs
    // before we have enough data to generate a frame for the
    // head ADU.
    Boolean needToEnqueue = True;

    if (!queueInfo.pendingFrames.isEmpty()) {
      SegmentEl* curADU = queueInfo.pendingFrames.head();
      int endOfHeadFrame = curADU->m_segment.m_mp3FrameSize -
                           curADU->m_segment.m_headerSize -
                           curADU->m_segment.m_sideInfoSize;
      int frameOffset = 0;
      while (1) {
        int endOfData = frameOffset - curADU->m_segment.m_backPointer +
                        curADU->m_segment.m_aduDataSize;
        if (endOfData >= endOfHeadFrame) {
          // We have enough data to generate a
          // frame.
          needToEnqueue = False;
          break;
        }

        frameOffset += curADU->m_segment.m_mp3FrameSize -
                       curADU->m_segment.m_headerSize -
                       curADU->m_segment.m_sideInfoSize;
        if (curADU == queueInfo.pendingFrames.tail()) break;
        curADU = queueInfo.pendingFrames.next(curADU);
      }
    }
    return needToEnqueue;
  }
  static void generateFrameFromHeadADU(QueueInfo& queueInfo) {
    SegmentEl* curADU = queueInfo.pendingFrames.head();

    //// Output the header and side info:
    // output(curADU.header);
    // output(curADU.sideInfo);

    // Begin by zeroing out the rest of the frame, in case the
    // ADU data doesn't fill it in completely:
    int endOfHeadFrame = curADU->m_segment.m_mp3FrameSize -
                         curADU->m_segment.m_headerSize -
                         curADU->m_segment.m_sideInfoSize;

    // output("endOfHeadFrame" zero bytes);

    // Fill in the frame with appropriate ADU data from this and
    // subsequent ADUs:
    int frameOffset = 0;
    int toOffset = 0;

    while (toOffset < endOfHeadFrame) {
      int startOfData = frameOffset - curADU->m_segment.m_backPointer;
      if (startOfData > endOfHeadFrame) {
        break;  // no more ADUs are needed
      }
      int endOfData = startOfData + curADU->m_segment.m_aduDataSize;
      if (endOfData > endOfHeadFrame) {
        endOfData = endOfHeadFrame;
      }
      int fromOffset;
      if (startOfData <= toOffset) {
        fromOffset = toOffset - startOfData;
        startOfData = toOffset;
        if (endOfData < startOfData) {
          endOfData = startOfData;
        }
      } else {
        fromOffset = 0;

        // leave some zero bytes beforehand:
        toOffset = startOfData;
      }

      int bytesUsedHere = endOfData - startOfData;

      /*output(starting at offset "toOffset",
             "bytesUsedHere" bytes from "&curADU.frameData[fromOffset]");*/

      toOffset += bytesUsedHere;

      frameOffset += curADU->m_segment.m_mp3FrameSize -
                     curADU->m_segment.m_headerSize -
                     curADU->m_segment.m_sideInfoSize;
      curADU = queueInfo.pendingFrames.next(curADU);
    }

    queueInfo.pendingFrames.dequeue();
  }
  static void insertDummyADUsIfNecessary(QueueInfo& queueInfo) {
    // The tail segment (ADU) is assumed to have been recently
    // enqueued.  If its backpointer would overlap the data
    // of the previous ADU, then we need to insert one or more
    // empty, 'dummy' ADUs ahead of it.  (This situation
    // should occur only if an intermediate ADU was missing
    // -- e.g., due to packet loss.)
    while (1) {
      SegmentEl* tailADU = queueInfo.pendingFrames.tail();
      int prevADUend;  // relative to the start of the tail ADU

      if (queueInfo.pendingFrames.head() != tailADU) {
        // there is a previous ADU
        SegmentEl* prevADU = queueInfo.pendingFrames.previous(tailADU);
        prevADUend = prevADU->m_segment.m_mp3FrameSize +
                     prevADU->m_segment.m_backPointer -
                     prevADU->m_segment.m_headerSize -
                     prevADU->m_segment.m_sideInfoSize;
        if (prevADU->m_segment.m_aduDataSize > prevADUend) {
          // this shouldn't happen if the
          // previous ADU was well-formed
          prevADUend = 0;
        } else {
          prevADUend -= prevADU->m_segment.m_aduDataSize;
        }
      } else {
        prevADUend = 0;
      }

      if (tailADU->m_segment.m_backPointer > prevADUend) {
        // Insert a 'dummy' ADU in front of the tail.
        // This ADU can have the same "header" (and thus,
        // "mp3FrameSize") as the tail ADU, but should
        // have a "backpointer" of "prevADUend", and
        // an "aduDataSize" of zero.  The simplest
        // way to do this is to copy the "sideInfo" from
        // the tail ADU, replace the value of
        // "main_data_begin" with "prevADUend", and set
        // all of the "part2_3_length" fields to zero.
      } else {
        break;  // no more dummy ADUs need to be
                // inserted
      }
    }
  }
};
}  // namespace ADU