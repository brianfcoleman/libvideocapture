#ifndef VIDEO_CAPTURE_SAMPLE_SOURCE_H
#define VIDEO_CAPTURE_SAMPLE_SOURCE_H

namespace VideoCapture {

#include "boost/cstdint.hpp"
#include "boost/shared_ptr.hpp"
#include "SampleQueue.hpp"

template<typename Sample, typename SampleAllocator> class SampleSource {
 public:
  typedef Sample SampleType;
  typedef SampleAllocator SampleAllocatorType;
  typedef typename SampleType::SampleFormatType SampleFormatType;
  typedef typename SampleAllocatorType::SizeType SizeType;
  typedef SampleQueue<SampleType> SampleQueueType;
  typedef boost::shared_ptr<SampleQueueType> SampleQueueSharedPtr;
  SampleSource(
      const SizeType maxCountAllocatedSamples,
      const SampleFormatType& sampleFormat,
      const SampleQueueSharedPtr& pOutputSampleQueue,
      const SampleQueueSharedPtr& pRecycledSampleQueue)
      : m_sampleAllocator(maxCountAllocatedSamples, sampleFormat),
        m_pOutputSampleQueue(pOutputSampleQueue),
        m_pRecycledSampleQueue(pRecycledSampleQueue),
        m_sampleIndex(0) {

  }

  SampleType allocateSample() {
    ++m_sampleIndex;
    SampleType sample(m_sampleAllocator.allocateSample(m_sampleIndex));
    if (sample) {
      return sample;
    }
    SampleType recycledSample(m_pRecycledSampleQueue->removeSample());
    return recycledSample;
  }

  SampleQueueSharedPtr outputSampleQueue() {
    return m_pOutputSampleQueue;
  }

  SampleQueueSharedPtr recycledSampleQueue() {
    return m_pRecycledSampleQueue;
  }

 private:
  SampleAllocatorType m_sampleAllocator;
  SampleQueueSharedPtr m_pOutputSampleQueue;
  SampleQueueSharedPtr m_pRecycledSampleQueue;
  boost::uint32_t m_sampleIndex;

} // VideoCapture
#endif // VIDEO_CAPTURE_SAMPLE_SOURCE_H
