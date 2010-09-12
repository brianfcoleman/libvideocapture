#ifndef VIDEO_CAPTURE_SAMPLE_QUEUE_IMPL_H
#define VIDEO_CAPTURE_SAMPLE_QUEUE_IMPL_H

#include <deque>
#include "boost/shared_ptr.hpp"
#include "SampleQueue.hpp"

namespace VideoCapture {

template<typename Sample> class SampleQueueImpl : public SampleQueue<Sample> {
 public:
  typedef typename SampleQueue::SampleType SampleType;
  typedef std::deque<SampleType> SampleContainerType;
  typedef boost::shared_ptr<SampleContainerType> SampleContainerSharedPtr;
  typedef SampleContainerType::size_type SizeType;

  explicit SampleQueueImpl(const SizeType capacity)
      : m_pSampleContainer(new SampleContainerType(capacity)) {

  }

  void addSample(const SampleType& sample) {
    m_pSampleContainer->push_front(sample);
  }

  SampleType removeSample() {
    if (m_pSampleContainer->empty()) {
      SampleType sample;
      return sample;
    }
    SampleType sample(m_pSampleContainer->back());
    m_pSampleContainer->pop_back();
    return sample;
  }

 private:
  SampleContainerSharedPtr m_pSampleContainer;

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_QUEUE_IMPL_H
