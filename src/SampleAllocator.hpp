#ifndef VIDEO_CAPTURE_SAMPLE_ALLOCATOR_H
#define VIDEO_CAPTURE_SAMPLE_ALLOCATOR_H

#include <deque>
#include "boost/cstdint.hpp"
#include "boost/shared_ptr.hpp"

namespace VideoCapture {

template<
    typename Sample,
    typename SampleDataFactory,
    typename SampleFormat> class SampleAllocator {
 public:
  typedef typename SampleDataFactory::SampleDataType SampleDataType;
  typedef typename SampleDataFactory::SampleDataSharedPtr SampleDataSharedPtr;
  typedef typename std::deque<SampleDataSharedPtr> SampleDataContainerType;
  typedef typename SampleDataContainerType::size_type SizeType;
  SampleAllocator(
      SizeType maxCountAllocatedSamples,
      SampleFormat sampleFormat)
      : m_pSampleDataContainer(
          new SampleDataContainerType(maxCountAllocatedSamples)),
        m_sampleFormat(sampleFormat) {
      allocateSampleData(maxCountAllocatedSamples);
  }

  Sample allocateSample(boost::uint32_t sampleIndex) {
    if (!m_sampleFormat) {
      Sample sample;
      return sample;
    }
    if (m_pSampleDataContainer->empty()) {
      Sample sample;
      return sample;
    }
    SampleDataSharedPtr pSampleData(m_pSampleDataContainer->back());
    m_pSampleDataContainer->pop_back();
    Sample sample(sampleIndex, m_sampleFormat, pSampleData);
    return sample;
  }

  SampleFormat sampleFormat() const {
    return m_sampleFormat;
  }
 private:
  boost::shared_ptr<SampleDataContainerType> m_pSampleDataContainer;
  SampleFormat m_sampleFormat;
  void allocateSampleData(SizeType maxCountAllocatedSamples) {
    if (!m_sampleFormat) {
      return;
    }
    SampleDataFactory sampleDataFactory;
    for (SizeType i = 0; i < maxCountAllocatedSamples; ++i) {
      SampleDataSharedPtr pSampleData(
          sampleDataFactory.createSampleData(m_sampleFormat));
      m_pSampleDataContainer->push_front(pSampleData);
    }
  }
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_ALLOCATOR_H
