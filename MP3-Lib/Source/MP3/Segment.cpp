#include "Segment.h"

namespace Segment {
void SegmentQueue::enqueue(SegmentEl* seg) {
  if (!m_head) {
    m_head = seg;
    m_tail = seg;
  } else {
    SegmentEl* prevTail = m_tail;
    seg->m_next = prevTail;
    prevTail->m_prev = seg;
    m_tail = seg;
  }
}
SegmentEl* SegmentQueue::dequeue() {
  if (!m_head) {
    return nullptr;
  }
  SegmentEl* res = m_head;
  SegmentEl* newHead = m_head->m_prev;
  m_head->m_prev = nullptr;
  newHead->m_next = nullptr;
  m_head = newHead;
  return res;
}
bool SegmentQueue::isEmpty() { return (m_head == nullptr) ? false : true; }
SegmentEl* SegmentQueue::head() { return m_head; }
SegmentEl* SegmentQueue::tail() { return m_tail; }
SegmentEl* SegmentQueue::previous(SegmentEl* seg) { return seg->m_prev; }
SegmentEl* SegmentQueue::next(SegmentEl* seg) { return seg->m_next; }
uint64_t SegmentQueue::totalDataSize() {
  SegmentEl* node = m_head;
  uint64_t res = 0;
  while (node != nullptr) {
    res += node->m_segment.m_frameDataSize;
    node = node->m_prev;
  }
  return res;
}
}  // namespace Segment