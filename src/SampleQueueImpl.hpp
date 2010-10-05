#ifndef VIDEO_CAPTURE_SAMPLE_QUEUE_IMPL_H
#define VIDEO_CAPTURE_SAMPLE_QUEUE_IMPL_H

#include <deque>
#include "SampleQueue.hpp"

namespace VideoCapture {

template<typename Sample> class SampleQueueImpl : public SampleQueue<Sample> {
 public:
  typedef SampleQueue<Sample> SampleQueueType;
  typedef typename SampleQueueType::SampleType SampleType;
  typedef std::deque<SampleType> SampleContainerType;
  typedef typename SampleContainerType::size_type SizeType;

  explicit SampleQueueImpl(const SizeType capacity)
      : m_sampleContainer() {

  }

  void addSample(const SampleType& sample) {
    m_sampleContainer.push_front(sample);
  }

  SampleType removeSample() {
    if (m_sampleContainer.empty()) {
      SampleType sample;
      return sample;
    }
    SampleType sample(m_sampleContainer.back());
    m_sampleContainer.pop_back();
    return sample;
  }

 private:
  SampleContainerType m_sampleContainer;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_QUEUE_IMPL_H
