#ifndef VIDEO_CAPTURE_SAMPLE_STREAM_BUILDER_H
#define VIDEO_CAPTURE_SAMPLE_STREAM_BUILDER_H

#include "boost/shared_ptr.hpp"
#include "Sample.hpp"
#include "SampleSource.hpp"
#include "SampleSink.hpp"
#include "SampleConverter.hpp"
#include "PassThroughSampleConverter.hpp"
#include "SampleProcessor.hpp"
#include "SampleProducer.hpp"
#include "SampleStream.hpp"
#include "SampleConsumerFactory.hpp"

namespace VideoCapture {

template<
  typename FirstSample,
  typename LastSample> class SampleStreamBuilder {
 public:
  typedef FirstSample FirstSampleType;
  typedef LastSample LastSampleType;
  typedef SampleSource<FirstSampleType> SampleSourceType;
  typedef SampleSink<LastSampleType> SampleSinkType;
  typedef SampleStreamImplBuilder<
    SampleSourceType,
    SampleSinkType> SampleStreamImplBuilderType;
  typedef SampleStream<
    SampleSourceType,
    SampleSinkType> SampleStreamType;
  typedef typename SampleStreamType::ProcessorThreadType ProcessorThreadType;
  typedef typename SampleStreamType::ProcessorThreadSharedPtr
  ProcessorThreadSharedPtr;
  typedef std::size_t SizeType;
  typedef SampleConsumer<LastSampleType> SampleConsumerType;
  typedef typename SampleConsumerType::SampleConsumerCallback
  SampleConsumerCallback;

  static const SizeType s_kDefaultMaxCountAllocatedSamples = 8;

  SampleStreamBuilder(const SizeType maxCountAllocatedSamples)
      : m_maxCountAllocatedSamples(maxCountAllocatedSamples) {

  }

  bool isReadyToBuild() const {
    return m_sampleStreamImplBuilder.isReadyToBuild();
  }

  SampleStreamType build() {
    if (!isReadyToBuild()) {
      SampleStreamType emptySampleStream;
      return emptySampleStream;
    }

    ProcessorThreadSharedPtr pProcessorThread(
        new ProcessorThreadType(m_sampleStreamImplBuilder));
    SampleStreamType sampleStream(pProcessorThread);
    return sampleStream;
  }

  template<typename CaptureDevice> SampleSourceType connectSampleSource(
      CaptureDevice& captureDevice) {
    typedef CaptureDevice CaptureDeviceType;
    typedef typename SampleSourceType::SampleFormatType SampleFormatType;
    typedef typename SampleSourceType::SampleQueuePairType SampleQueuePairType;
    typedef typename CaptureDeviceType::RawSampleDataType RawSampleDataType;
    typedef typename CaptureDeviceType::SampleProducerCallbackType
        SampleProducerCallbackType;
    typedef typename CaptureDeviceType::SampleProducerCallbackSharedPtr
        SampleProducerCallbackSharedPtr;
    typedef SampleProducer<
      SampleSourceType,
      RawSampleDataType> SampleProducerType;
    typedef typename SampleProducerType::SampleProducerSharedPtr
        SampleProducerSharedPtr;
    typedef typename SampleSourceType::SampleType SampleType;
    typedef typename SampleType::SampleDataType SampleDataType;
    typedef typename SampleDataType::RawSampleDataConverter
        RawSampleDataConverter;
    typedef typename SampleProducerType::RawSampleDataToSampleConverter
        RawSampleDataToSampleConverter;

    // TODO Change the API of VideoCaptureDevice to support currentSampleFormat
    // instead of currentVideoFormat
    SampleFormatType currentCaptureDeviceSampleFormat(
        captureDevice.currentVideoFormat());
#ifdef DEBUG
    bool isFormatInitialized = currentCaptureDeviceSampleFormat.isInitialized();
    std::cout << "SampleStreamBuilder::connectSampleSource current format";
    std::cout << (isFormatInitialized ? " is " : " is not ");
    std::cout << "initialized" << std::endl;
#endif
    SampleQueuePairType threadSafeSampleQueuePair(
        threadSafeRecyclingSampleQueuePair<
          SampleType>(m_maxCountAllocatedSamples));
    SampleSourceType sampleSource(
        m_maxCountAllocatedSamples,
        currentCaptureDeviceSampleFormat,
        threadSafeSampleQueuePair);
    RawSampleDataConverter rawSampleDataConverter;
    RawSampleDataToSampleConverter rawSampleDataToSampleConverter(
        rawSampleDataConverter);
    SampleProducerType sampleProducer(
        sampleSource,
        rawSampleDataToSampleConverter);
    SampleProducerCallbackSharedPtr pSampleProducer(
        new SampleProducerCallbackType(sampleProducer));
    captureDevice.addSampleProducerCallback(pSampleProducer);
    m_sampleStreamImplBuilder.addSampleSource(sampleSource);
    return sampleSource;
  }

  template<
   typename PreviousSampleStreamStageType,
   typename ConvertedSampleType> SampleProcessor<
   typename PreviousSampleStreamStageType::SampleType,
    ConvertedSampleType> connectSampleConverterToPreviousSampleStage(
      PreviousSampleStreamStageType previousSampleStreamStage,
      boost::shared_ptr<
      SampleConverter<
      typename PreviousSampleStreamStageType::SampleType,
      ConvertedSampleType>> pSampleConverter) {
    typedef typename RecyclingSampleQueuePair<ConvertedSampleType>
        ConvertedSampleQueuePairType;
    ConvertedSampleQueuePairType convertedSampleQueuePair(
        singleThreadedRecyclingSampleQueuePair(m_maxCountAllocatedSamples));
    return connectSampleConverterToPreviousSampleStage(
        previousSampleStreamStage,
        pSampleConverter,
        convertedSampleQueuePair);
  }

  template<
    typename PreviousSampleStreamStageType> SampleConsumerType
  connectSampleSinkToPreviousSampleStage(
      PreviousSampleStreamStageType previousSampleStreamStage,
      SampleConsumerCallback sampleConsumerCallback) {
    typedef typename SampleSinkType::SampleQueuePairType SampleQueuePairType;
    typedef typename SampleSinkType::SampleType SampleType;
    typedef typename SampleType::SampleFormatType SampleFormatType;
    typedef PassThroughSampleConverter<SampleType> SampleConverterType;
    typedef typename SampleConverterType::SampleConverterSharedPtr
        SampleConverterSharedPtr;
    typedef SampleProcessor<SampleType, SampleType> SampleProcessorType;
    typedef SampleConsumerFactory<SampleType> SampleConsumerFactoryType;

    SampleConverterSharedPtr pSampleConverter(
        sampleConverterSharedPtr<SampleType, SampleType, SampleConverterType>());
    SampleQueuePairType sampleQueuePair(
        threadSafeRecyclingSampleQueuePair<
          SampleType>(m_maxCountAllocatedSamples));
    SampleProcessorType sampleProcessor(
        connectSampleConverterToPreviousSampleStage(
            previousSampleStreamStage,
            pSampleConverter,
            sampleQueuePair));
    SampleSinkType sampleSink(sampleQueuePair);
    m_sampleStreamImplBuilder.addSampleSink(sampleSink);
    SampleConsumerFactoryType sampleConsumerFactory(
        sampleSink,
        sampleConsumerCallback);
    SampleConsumerType sampleConsumer(sampleConsumerFactory());
    return sampleConsumer;
  }

  operator bool() const {
    return isReadyToBuild();
  }

  SampleStreamType operator()() {
    return build();
  }

 private:
  template<
   typename PreviousSampleStreamStageType,
   typename ConvertedSampleType> SampleProcessor<
   typename PreviousSampleStreamStageType::SampleType,
    ConvertedSampleType> connectSampleConverterToPreviousSampleStage(
      PreviousSampleStreamStageType previousSampleStreamStage,
      boost::shared_ptr<
      SampleConverter<
      typename PreviousSampleStreamStageType::SampleType,
      ConvertedSampleType>> pSampleConverter,
      RecyclingSampleQueuePair<ConvertedSampleType> convertedSampleQueuePair) {
    typedef typename PreviousSampleStreamStageType::SampleType InputSampleType;
    typedef typename ConvertedSampleType OutputSampleType;
    typedef SampleProcessor<
      InputSampleType,
      OutputSampleType> SampleProcessorType;
    typedef typename SampleProcessorType::InputSampleFormatType
        InputSampleFormatType;
    typedef typename SampleProcessorType::InputSampleQueuePairType
        InputSampleQueuePairType;

    if (!pSampleConverter) {
      SampleProcessorType sampleProcessor;
      return sampleProcessor;
    }
    InputSampleFormatType inputSampleFormat(
        previousSampleStreamStage.sampleFormat());
    InputSampleQueuePairType inputSampleQueuePair(
        previousSampleStreamStage.sampleQueuePair());
    SampleProcessorType sampleProcessor(
        inputSampleFormat,
        pSampleConverter,
        inputSampleQueuePair,
        convertedSampleQueuePair,
        m_maxCountAllocatedSamples);
    m_sampleStreamImplBuilder.addSampleProcessor(sampleProcessor);
    return sampleProcessor;
  }

  SizeType m_maxCountAllocatedSamples;
  SampleStreamImplBuilderType m_sampleStreamImplBuilder;

};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_STREAM_BUILDER_H
