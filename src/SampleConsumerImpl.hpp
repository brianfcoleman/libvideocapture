#ifndef VIDEO_CAPTURE_SAMPLE_CONSUMER_IMPL_H
#define VIDEO_CAPTURE_SAMPLE_CONSUMER_IMPL_H

#include "boost/function.hpp"
#include "SampleSink.hpp"

namespace VideoCapture {

template<typename Sample> class SampleConsumerImpl {
 public:
  typedef Sample SampleType;
  typedef SampleType& SampleRef;
  typedef boost::function<void (SampleRef)> SampleConsumerCallback;
  typedef SampleSink<SampleType> SampleSinkType;

  SampleConsumerImpl(
      const SampleSinkType& sampleSink,
      const SampleConsumerCallback& sampleConsumerCallback)
      : m_sampleSink(sampleSink),
        m_sampleConsumerCallback(sampleConsumerCallback) {

  }

  void consumeSample() {
#ifndef DEBUG
    std::cout << "SampleConsumerImpl::consumeSample" << std::endl;
#endif
    SampleType sample(m_sampleSink.removeSample());
    if (!sample) {
      return;
    }

    m_sampleConsumerCallback(sample);

    m_sampleSink.recycleSample(sample);
  }

  void operator()() {
    consumeSample();
  }

 private:
  SampleSinkType m_sampleSink;
  SampleConsumerCallback m_sampleConsumerCallback;

};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_CONSUMER_IMPL_H
