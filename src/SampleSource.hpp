#ifndef VIDEO_CAPTURE_SAMPLE_SOURCE_H
#define VIDEO_CAPTURE_SAMPLE_SOURCE_H

#include "boost/cstdint.hpp"
#include "RecyclingSampleQueuePair.hpp"

namespace VideoCapture {

template<typename Sample> class SampleSource {
 public:
  typedef Sample SampleType;
  typedef typename SampleType::SampleAllocatorType SampleAllocatorType;
  typedef typename SampleType::SampleFormatType SampleFormatType;
  typedef typename SampleAllocatorType::SizeType SizeType;
  typedef typename SampleType::SampleFormatType SamplFormatType;
  typedef RecyclingSampleQueuePair<SampleType> SampleQueuePairType;
  typedef typename SampleQueuePairType::SampleQueueSharedPtr
  SampleQueueSharedPtr;

  SampleSource()
      : m_sampleIndex(0) {

  }

  SampleSource(
      const SizeType maxCountAllocatedSamples,
      const SampleFormatType& sampleFormat,
      const SampleQueuePairType& sampleQueuePair)
      : m_sampleAllocator(maxCountAllocatedSamples, sampleFormat),
        m_sampleQueuePair(sampleQueuePair),
        m_sampleIndex(0) {

  }

  SampleFormatType sampleFormat() {
    return m_sampleAllocator.sampleFormat();
  }

  SampleQueuePairType sampleQueuePair() {
    return m_sampleQueuePair;
  }

  SampleType allocateSample() {
#ifdef DEBUG
    std::cout << "SampleSource::allocateSample" << std::endl;
#endif
    if (!isInitialized()) {
      SampleType sample;
      return sample;
    }
    recycleSample();
    ++m_sampleIndex;
    SampleType sample(m_sampleAllocator.allocateSample(m_sampleIndex));
    return sample;
  }

  void addSample(SampleType& sample) {
    if (!sample) {
      return;
    }

    if (!isInitialized()) {
      return;
    }

    SampleQueueSharedPtr pSampleQueue(m_sampleQueuePair.sampleQueue());
    if (!pSampleQueue) {
      return;
    }

    pSampleQueue->addSample(sample);
  }

  bool isInitialized() const {
    if (!m_sampleAllocator) {
      return false;
    }

    return true;
  }

  operator bool() const {
    return isInitialized();
  }

 private:
  void recycleSample() {
    if (!isInitialized()) {
      return;
    }

    m_sampleAllocator.recycleSample(m_sampleQueuePair);
  }

  SampleAllocatorType m_sampleAllocator;
  SampleQueuePairType m_sampleQueuePair;
  boost::uint32_t m_sampleIndex;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_SOURCE_H
