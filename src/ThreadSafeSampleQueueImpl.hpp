#ifndef VIDEO_CAPTURE_THREAD_SAFE_SAMPLE_QUEUE_IMPL_H
#define VIDEO_CAPTURE_THREAD_SAFE_SAMPLE_QUEUE_IMPL_H

#include "boost/shared_ptr.hpp"
#include "SampleQueue.hpp"
#include "BoundedBuffer.hpp"

namespace VideoCapture {

template<typename Sample> class ThreadSafeSampleQueueImpl
    : public SampleQueue<Sample> {
 public:
  typedef typename SampleQueue::SampleType SampleType;
  typedef boost::bounded_buffer<SampleType> SampleContainerType;
  typedef boost::shared_ptr<SampleContainerType> SampleContainerSharedPtr;
  typedef SampleContainerType::size_type SizeType;

  explicit ThreadSafeSampleQueueImpl(const SizeType capacity)
      : m_pSampleContainer(new SampleContainerType(capacity)) {

  }

  void addSample(const SampleType& sample) {
    m_pSampleContainer->push_front(sample);
  }

  SampleType removeSample() {
    return m_pSampleContainer->pop_back();
  }

 private:
  SampleContainerSharedPtr m_pSampleContainer;

} // VideoCapture

#endif // VIDEO_CAPTURE_THREAD_SAFE_SAMPLE_QUEUE_IMPL_H
