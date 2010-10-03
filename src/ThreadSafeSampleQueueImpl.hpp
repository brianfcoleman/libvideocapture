#ifndef VIDEO_CAPTURE_THREAD_SAFE_SAMPLE_QUEUE_IMPL_H
#define VIDEO_CAPTURE_THREAD_SAFE_SAMPLE_QUEUE_IMPL_H

#include "SampleQueue.hpp"
#include "BoundedBuffer.hpp"

namespace VideoCapture {

template<typename Sample> class ThreadSafeSampleQueueImpl
    : public SampleQueue<Sample> {
 public:
  typedef SampleQueue<Sample> SampleQueueType;
  typedef typename SampleQueueType::SampleType SampleType;
  typedef bounded_buffer<SampleType> SampleContainerType;
  typedef typename SampleContainerType::size_type SizeType;

  explicit ThreadSafeSampleQueueImpl(const SizeType capacity)
      : m_sampleContainer(capacity) {

  }

  void addSample(const SampleType& sample) {
    m_sampleContainer.push_front(sample);
  }

  SampleType removeSample() {
    return m_sampleContainer.pop_back();
  }

 private:
  SampleContainerType m_sampleContainer;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_THREAD_SAFE_SAMPLE_QUEUE_IMPL_H
