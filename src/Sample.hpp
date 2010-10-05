#ifndef VIDEO_CAPTURE_SAMPLE_H
#define VIDEO_CAPTURE_SAMPLE_H

#include "boost/cstdint.hpp"
#include "boost/shared_ptr.hpp"
#include "SampleAllocator.hpp"

namespace VideoCapture {

template<typename SampleData> class Sample {
 public:
  typedef SampleData SampleDataType;
  typedef typename SampleDataType::SampleFormatType SampleFormatType;
  typedef Sample<SampleDataType> SampleType;
  typedef SampleAllocator<SampleType> SampleAllocatorType;
  typedef boost::shared_ptr<SampleDataType> SampleDataSharedPtr;

  Sample()
      : m_sampleIndex(0) {

  }

  Sample(
      const boost::uint32_t sampleIndex,
      const SampleFormatType& sampleFormat,
      const SampleDataSharedPtr& pSampleData)
      : m_sampleIndex(sampleIndex),
        m_sampleFormat(sampleFormat),
        m_pSampleData(pSampleData) {

  }

  boost::uint32_t sampleIndex() const {
    return m_sampleIndex;
  }

  SampleFormatType sampleFormat() const {
    return m_sampleFormat;
  }

  SampleDataSharedPtr sampleData() {
    return SampleDataSharedPtr(m_pSampleData);
  }

  bool isInitialized() const {
    if (!m_sampleFormat) {
      return false;
    }

    if (!m_pSampleData) {
#ifdef DEBUG
    std::cout << "Sample::isInitialized sample data null" << std::endl;
#endif
      return false;
    }

    return true;
  }

  operator bool() const {
    return isInitialized();
  }

 private:
  boost::uint32_t m_sampleIndex;
  SampleFormatType m_sampleFormat;
  SampleDataSharedPtr m_pSampleData;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_H
