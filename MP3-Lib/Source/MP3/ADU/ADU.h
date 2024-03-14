#pragma once
#include <MP3/Segment.h>

#include <MP3/MP3Internals.hh>
#include <bitset>
#include <cstdint>
#include <functional>
#include <string>
namespace ADU {
using namespace Segment;
template <QueueElement T>
struct QueueInfo {
  SegmentQueue<T> pendingFrames;
  uint64_t currentFrameNum{};
  uint64_t totalDataSizeBefore{0};
  uint64_t totalDataSizeAfter{0};
};
// int interleave_cycle[8] = {1, 3, 5, 7, 0, 2, 4, 6};
class ADU {
  int interleave_cycle[8] = {4, 0, 5, 1, 6, 2, 7, 3};
  using uint = unsigned int;

 public:
  static void interleaveFrames(QueueInfo<ADUFrameEl>& orgSeq,
                               std::vector<std::string>& vec) {
    SegmentQueue<ADUFrameEl>* beforeQ = &orgSeq.pendingFrames;
    ADUFrameEl* frame{beforeQ->head()};
    /*int interleaveIdx{0};
    int interleaveCycleNum{0};
    ADUFrameEl* cycleFrames[8]{};
    while (frame != nullptr) {
      cycleFrames[interleaveIdx] = frame;
      interleaveIdx = (interleaveIdx + 1) % 8;
      if (interleaveIdx == 0) {
        for (auto* el : cycleFrames) {
          std::string frame = el->m_frame.header +
                              getStringFromSizeInfo(el->m_frame.sideInfo,
                                                    el->m_frame.header, 32) +
                              el->m_frame.audioData;

          res.push_back(frame);
        }
        interleaveCycleNum += 1;
      }
      frame = frame->m_next;
    }*/
    while (frame != nullptr) {
      std::string frameString =
          frame->m_frame.header +
          getStringFromSizeInfo(frame->m_frame.sideInfo, frame->m_frame.header,
                                32) +
          frame->m_frame.audioData;
      vec.push_back(frameString);
      frame = frame->m_prev;
    }
  }
  static std::string addAduDescriptor(std::string& aduFrame) {
    // Assume packet will fit in 1500 MTU
    std::string res;
    int continuationFlag = 0;
    // 0 if <= 64 bytes else 1
    int descriptorTypeflag = 1;
    int aduSize = aduFrame.length();
    auto t = aduSize >> 6;
    std::bitset<8> left8bits(aduSize >> 6);
    std::bitset<6> right6bits(aduSize);
    res = std::to_string(continuationFlag) +
          std::to_string(descriptorTypeflag) + left8bits.to_string() +
          right6bits.to_string() + aduFrame;
    return res;
  }
  static QueueInfo<ADUFrameEl> MP3ToADU(std::vector<std::string>& frames,
                                        QueueInfo<SegmentEl>& queueInfo) {
    std::string res;
    ADUFrameEl* aduFrame;
    SegmentEl* newFrame;
    uint idx{0};
    QueueInfo<ADUFrameEl> ADUFrames;
    while (1) {
      if (idx == frames.size()) {
        return ADUFrames;
      }
      // Enqueue new MP3 Frames, until we have enough data to
      // generate the ADU for a frame:
      do {
        queueInfo.totalDataSizeBefore = queueInfo.pendingFrames.totalDataSize();
        if (idx == frames.size()) {
          return ADUFrames;
        }

        newFrame = new SegmentEl(frames[idx++]);
        queueInfo.pendingFrames.enqueue(newFrame);
        queueInfo.currentFrameNum += 1;
        queueInfo.totalDataSizeAfter = queueInfo.pendingFrames.totalDataSize();
      } while (
          queueInfo.totalDataSizeBefore < newFrame->m_segment.m_backPointer ||
          queueInfo.totalDataSizeAfter < newFrame->m_segment.m_aduDataSize);

      // We now have enough data to generate the ADU for the most
      // recently enqueued frame (i.e., the tail of the queue).
      // (The earlier frames in the queue -- if any -- must be
      // discarded, as we don't have enough data to generate
      // their ADUs.)
      SegmentEl* tailFrame = queueInfo.pendingFrames.tail();
      aduFrame = new ADUFrameEl();
      aduFrame->m_frame.header = tailFrame->m_segment.m_header;
      aduFrame->m_frame.sideInfo = tailFrame->m_segment.m_sideInfo;
      aduFrame->m_frame.audioData = "";
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
        aduFrame->m_frame.audioData += std::string(
            (char*)(&curFrame->m_segment.m_frameData[offset]), bytesUsedHere);

        ADUFrames.pendingFrames.enqueue(aduFrame);

        bytesToUse -= bytesUsedHere;
        offset = 0;
        curFrame = queueInfo.pendingFrames.next(curFrame);
      }
    }
  }

  static std::vector<std::string> ADUToMP3(
      QueueInfo<SegmentEl>& queueInfo, QueueInfo<ADUFrameEl>& queueInfoADU) {
    std::vector<std::string> res;
    SegmentEl* newADU;
    int aduIdx = 0;
    while (1) {
      while (needToGetAnADU(queueInfo)) {
        ADUFrameEl* fr = queueInfoADU.pendingFrames.dequeue();
        if (aduIdx == 7964) {
          int k = 1;
        }
        if (!fr) {
          return res;
        }
        aduIdx += 1;
        newADU = new SegmentEl(*fr);
        queueInfo.pendingFrames.enqueue(newADU);

        insertDummyADUsIfNecessary(queueInfo);
      }

      res.push_back(generateFrameFromHeadADU(queueInfo));
    }
  }

  static bool needToGetAnADU(QueueInfo<SegmentEl>& queueInfo) {
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
        if (endOfData <= endOfHeadFrame) {
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

  static std::string getStringFromSizeInfo(MP3SideInfo& si,
                                           std::string& headerStr,
                                           uint8_t sideInfoSize) {
    unsigned char* sideInfoStr = new unsigned char[sideInfoSize];
    const unsigned char* header =
        reinterpret_cast<const unsigned char*>(headerStr.c_str());
    MP3FrameParams fr;
    fr.hdr = ((unsigned)header[0] << 24) | ((unsigned)header[1] << 16) |
             ((unsigned)header[2] << 8) | (unsigned)header[3];
    fr.setParamsFromHeader();
    PutMP3SideInfoIntoFrame(si, fr, sideInfoStr);
    return std::string(reinterpret_cast<char*>(&sideInfoStr[0]), sideInfoSize);
  }

  static std::string generateFrameFromHeadADU(QueueInfo<SegmentEl>& queueInfo) {
    std::string res;
    SegmentEl* curADU = queueInfo.pendingFrames.head();

    //// Output the header and side info:
    // output(curADU.header);
    res += curADU->m_segment.m_header;
    // output(curADU.sideInfo);
    std::string sideInfoString = getStringFromSizeInfo(
        curADU->m_segment.m_sideInfo, curADU->m_segment.m_header,
        curADU->m_segment.m_sideInfoSize);

    res += sideInfoString;

    // Begin by zeroing out the rest of the frame, in case the
    // ADU data doesn't fill it in completely:
    int endOfHeadFrame = curADU->m_segment.m_mp3FrameSize -
                         curADU->m_segment.m_headerSize -
                         curADU->m_segment.m_sideInfoSize;

    // output("endOfHeadFrame" zero bytes);
    std::string frameDataStr(endOfHeadFrame, '\0');

    // Fill in the frame with appropriate ADU data from this and
    // subsequent ADUs:
    int frameOffset = 0;
    int toOffset = 0;

    while (toOffset < endOfHeadFrame && curADU) {
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

      frameDataStr.replace(toOffset, bytesUsedHere,
                           curADU->m_segment.m_frameData.substr(fromOffset));

      res += frameDataStr;

      toOffset += bytesUsedHere;

      frameOffset += curADU->m_segment.m_mp3FrameSize -
                     curADU->m_segment.m_headerSize -
                     curADU->m_segment.m_sideInfoSize;
      curADU = queueInfo.pendingFrames.next(curADU);
    }

    queueInfo.pendingFrames.dequeue();
    return res;
  }
  static void insertDummyADUsIfNecessary(QueueInfo<SegmentEl>& queueInfo) {
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
        SegmentEl* dummySegment = new SegmentEl();
        dummySegment->m_segment.m_header = tailADU->m_segment.m_header;
        dummySegment->m_segment.m_headerSize = tailADU->m_segment.m_headerSize;
        dummySegment->m_segment.m_mp3FrameSize =
            tailADU->m_segment.m_mp3FrameSize;
        dummySegment->m_segment.m_backPointer = prevADUend;
        dummySegment->m_segment.m_aduDataSize = 0;
        dummySegment->m_segment.m_sideInfo = tailADU->m_segment.m_sideInfo;
        dummySegment->m_segment.m_sideInfoSize =
            tailADU->m_segment.m_sideInfoSize;
        dummySegment->m_segment.m_sideInfo.main_data_begin = prevADUend;
        dummySegment->m_segment.m_sideInfo.ch[0].gr[0].part2_3_length = 0;
        dummySegment->m_segment.m_sideInfo.ch[0].gr[1].part2_3_length = 0;
        dummySegment->m_segment.m_sideInfo.ch[1].gr[0].part2_3_length = 0;
        dummySegment->m_segment.m_sideInfo.ch[1].gr[1].part2_3_length = 0;

        // Insert dummy segment after tail

        SegmentEl* tailNext = tailADU->m_next;
        dummySegment->m_prev = tailADU;
        dummySegment->m_next = tailNext;
        tailNext->m_prev = dummySegment;
        tailADU->m_next = dummySegment;

      } else {
        break;  // no more dummy ADUs need to be
                // inserted
      }
    }
  }
};
}  // namespace ADU