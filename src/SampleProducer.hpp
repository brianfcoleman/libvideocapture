#ifndef VIDEO_CAPTURE_SAMPLE_PRODUCER_H
#define VIDEO_CAPTURE_SAMPLE_PRODUCER_H

#include "boost/function.hpp"

namespace VideoCapture {

template<typename SampleSource, typename RawSampleData> class SampleProducer {
 public:
  typedef SampleSource SampleSourceType;
  typedef RawSampleData RawSampleDataType;
  typedef typename SampleSourceType::SampleType SampleType;
  typedef SampleType& SampleRef;
  typedef boost::function<(RawSampleDataType, SampleRef)> RawSampleDataConverter;
  typedef typename SampleSourceType::SampleQueueSharedPtr SampleQueueSharedPtr;

  SampleProducer(
      const SampleSourceType& sampleSource,
      RawSampleDataConverter converter)
      : m_sampleSource(sampleSource),
        m_converter(converter) {

  };

  void produceSample(RawSampleData rawData) {
    SampleType sample(m_sampleSource.allocateSample());
    if (!sample) {
      return;
    }
    m_converter(rawData, sample);
    SampleQueueSharedPtr pOutputSampleQueue(m_sampleSource.outputSampleQueue());
    if (!pOutputSampleQueue) {
      return;
    }
    pOutputSampleQueue->addSample(sample);
  }

  void operator()(RawSampleData rawData) {
    produceSample(rawData);
  }

 private:
  SampleSourceType m_sampleSource;
  RawSampleDataConverter m_converter;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_PRODUCER_H
