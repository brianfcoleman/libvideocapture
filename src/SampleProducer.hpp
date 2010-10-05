#ifndef VIDEO_CAPTURE_SAMPLE_PRODUCER_H
#define VIDEO_CAPTURE_SAMPLE_PRODUCER_H

#include "boost/shared_ptr.hpp"
#include "boost/function.hpp"

namespace VideoCapture {

template<
  typename SampleSource,
  typename RawSampleData> class SampleProducer  {
 public:
  typedef SampleSource SampleSourceType;
  typedef RawSampleData RawSampleDataType;
  typedef typename SampleSourceType::SampleType SampleType;
  typedef SampleType& SampleRef;
  typedef boost::function<
    void (RawSampleDataType, SampleRef)> RawSampleDataToSampleConverter;
  typedef typename SampleSourceType::SampleQueueSharedPtr SampleQueueSharedPtr;
  typedef SampleProducer<SampleSourceType, RawSampleDataType> SampleProducerType;
  typedef boost::shared_ptr<SampleProducerType> SampleProducerSharedPtr;

  SampleProducer(
      const SampleSourceType& sampleSource,
      RawSampleDataToSampleConverter converter)
      : m_sampleSource(sampleSource),
        m_converter(converter) {

  }

  void produceSample(RawSampleData rawData) {
    SampleType sample(m_sampleSource.allocateSample());
    if (!sample) {
      return;
    }
#ifdef DEBUG
    std::cout << "SampleProducer::produceSample converting sample" << std::endl;
#endif
    m_converter(rawData, sample);
#ifdef DEBUG
    std::cout << "SampleProducer::produceSample adding sample" << std::endl;
#endif
    m_sampleSource.addSample(sample);
  }

  void operator()(RawSampleData rawData) {
    produceSample(rawData);
  }

 private:
  SampleSourceType m_sampleSource;
  RawSampleDataToSampleConverter m_converter;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_PRODUCER_H
