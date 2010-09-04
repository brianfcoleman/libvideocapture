#ifndef VIDEO_CAPTURE_SAMPLE_H
#define VIDEO_CAPTURE_SAMPLE_H

#include "boost/cstdint.hpp"
#include "boost/shared_ptr.hpp"

namespace VideoCapture {

template<
  typename SampleData,
  typename SampleFormat>
class Sample {
 public:
  typedef typename SampleData SampleDataType;
  typedef typename SampleFormat SampleFormatType;
  typedef typename boost::shared_ptr<SampleData> SampleDataSharedPtr;
  Sample()
      : m_sampleIndex(0) {

  }

  Sample(
      const boost::uint32_t sampleIndex,
      const SampleFormat& sampleFormat,
      const SampleDataSharedPtr& pSampleData)
      : m_sampleIndex(sampleIndex),
        m_sampleFormat(sampleFormat),
        m_pSampleData(pSampleData) {

  }

  boost::uint32_t sampleIndex() const {
    return m_sampleIndex;
  }

  SampleFormat sampleFormat() const {
    return m_sampleFormat;
  }

  /**
   * The sample data is recycled as soon as the sample goes out of scope.
   * Do not retain a reference to sample data
   * or attempt to use it after it has been recycled.
   */
  SampleDataSharedPtr sampleData() {
    return SampleDataSharedPtr(m_pSampleData);
  }

  bool isInitialized() const {
    if (!m_sampleFormat) {
      return false;
    }

    if (!m_pSampleData) {
      return false;
    }

    return true;
  }

  operator bool() const {
    return isInitialized();
  }

 private:
  boost::uint32_t m_sampleIndex;
  SampleFormat m_sampleFormat;
  SampleDataSharedPtr m_pSampleData;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_H
