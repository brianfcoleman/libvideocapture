#ifndef VIDEO_CAPTURE_SAMPLE_CONSUMER_FACTORY_H
#define VIDEO_CAPTURE_SAMPLE_CONSUMER_FACTORY_H

#include "SampleConsumer.hpp"

namespace VideoCapture {

template<typename Sample> class SampleConsumerFactory {
 public:
  typedef Sample SampleType;
  typedef SampleConsumer<SampleType> SampleConsumerType;
  typedef typename SampleConsumerImplFactory<SampleType>
  SampleConsumerImplFactoryType;
  typedef typename SampleConsumerType::SampleConsumerCallback
  SampleConsumerCallback;
  typedef typename SampleConsumerType::SampleSinkType
  SampleSinkType;
  typedef typename SampleConsumerType::ProcessorThreadType ProcessorThreadType;
  typedef typename SampleConsumerType::ProcessorThreadSharedPtr
  ProcessorThreadSharedPtr;

  SampleConsumerFactory(
      const SampleSinkType& sampleSink,
      const SampleConsumerCallback& sampleConsumerCallback)
      : m_sampleConsumerImplFactory(sampleSink, sampleConsumerCallback) {

  }

  SampleConsumerType createSampleConsumer() const {
    ProcessorThreadSharedPtr pProcessorThread(
        new ProcessorThreadType(m_sampleConsumerImplFactory));
    SampleConsumerType sampleConsumer(pProcessorThread);
    return sampleConsumer;
  }

  SampleConsumerType operator()() const {
    return createSampleConsumer();
  }

 private:
  SampleConsumerImplFactoryType m_sampleConsumerImplFactory;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_CONSUMER_FACTORY_H
