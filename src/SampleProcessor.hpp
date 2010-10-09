#ifndef VIDEO_CAPTURE_SAMPLE_PROCESSOR_H
#define VIDEO_CAPTURE_SAMPLE_PROCESSOR_H

#include "boost/cstdint.hpp"
#include "SampleConverter.hpp"
#include "RecyclingSampleQueuePair.hpp"

namespace VideoCapture {

template<typename InputSample,
         typename OutputSample> class SampleProcessor {
 public:
  typedef InputSample InputSampleType;
  typedef OutputSample OutputSampleType;
  typedef typename OutputSampleType::SampleAllocatorType SampleAllocatorType;
  typedef typename SampleAllocatorType::SizeType SizeType;
  typedef SampleConverter<InputSampleType, OutputSampleType> SampleConverterType;
  typedef typename SampleConverterType::SampleConverterSharedPtr
  SampleConverterSharedPtr;
  typedef typename SampleConverterType::InputSampleFormatType
  InputSampleFormatType;
  typedef typename SampleConverterType::OutputSampleFormatType
  OutputSampleFormatType;
  typedef typename  SampleConverterType::InputSampleRef InputSampleRef;
  typedef typename SampleConverterType::OutputSampleRef OutputSampleRef;
  typedef RecyclingSampleQueuePair<InputSampleType> InputSampleQueuePairType;
  typedef typename InputSampleQueuePairType::SampleQueueSharedPtr
  InputSampleQueueSharedPtr;
  typedef RecyclingSampleQueuePair<OutputSampleType> OutputSampleQueuePairType;
  typedef typename OutputSampleQueuePairType::SampleQueueSharedPtr
  OutputSampleQueueSharedPtr;

  SampleProcessor() {

  }

  /**
   * This assumes that pSampleConverter is valid
   */
  SampleProcessor(
      const InputSampleFormatType inputSampleFormat,
      const SampleConverterSharedPtr& pSampleConverter,
      const InputSampleQueuePairType& inputSampleQueuePair,
      const OutputSampleQueuePairType& outputSampleQueuePair,
      const SizeType maxCountAllocatedSamples)
      : m_pSampleConverter(pSampleConverter),
        m_inputSampleQueuePair(inputSampleQueuePair),
        m_outputSampleQueuePair(outputSampleQueuePair),
        m_sampleAllocator(
            maxCountAllocatedSamples,
            m_pSampleConverter->convertedSampleFormat(inputSampleFormat)) {
#ifdef DEBUG
    OutputSampleFormatType convertedSampleFormat(
        m_sampleAllocator.sampleFormat());
    std::cout << "SampleProcessor " << (inputSampleFormat ? "valid" : "invalid");
    std::cout << " input sample format" << std::endl;
    std::cout << (convertedSampleFormat ? "valid" : "invalid");
    std::cout << " converted sample format" << std::endl;
#endif
  }

  OutputSampleFormatType sampleFormat() {
    return m_sampleAllocator.sampleFormat();
  }

  OutputSampleQueuePairType sampleQueuePair() {
    return m_outputSampleQueuePair;
  }

  void processSample() {
#ifdef DEBUG
    std::cout << "SampleProcessor::processSample" << std::endl;
#endif
    if (!isInitialized()) {
      return;
    }

    InputSampleQueueSharedPtr pInputSampleQueue(
        m_inputSampleQueuePair.sampleQueue());
    if (!pInputSampleQueue) {
      return;
    }
    InputSampleQueueSharedPtr pRecycledInputSampleQueue(
        m_inputSampleQueuePair.recycledSampleQueue());
    if (!pRecycledInputSampleQueue) {
      return;
    }
    OutputSampleQueueSharedPtr pOutputSampleQueue(
        m_outputSampleQueuePair.sampleQueue());
    if (!pOutputSampleQueue) {
      return;
    }

    InputSampleType inputSample(pInputSampleQueue->removeSample());
    if (!inputSample) {
      return;
    }
    boost::uint32_t sampleIndex = inputSample.sampleIndex();
#ifdef DEBUG
    std::cout << "SampleProcessor::processSample removed input sample";
    std::cout << std::endl;
#endif

    OutputSampleType outputSample(allocateOutputSample(sampleIndex));
    if (!outputSample) {
      return;
    }

    m_pSampleConverter->operator()(inputSample, outputSample);
    pRecycledInputSampleQueue->addSample(inputSample);
    pOutputSampleQueue->addSample(outputSample);
  }

  bool isInitialized() const {
    if (!m_sampleAllocator) {
      return false;
    }

    if (!m_pSampleConverter) {
      return false;
    }

    return true;
  }

  void operator()() {
    processSample();
  }

  operator bool() const {
    return isInitialized();
  }

 private:
  void recycleOutputSample() {
    if (!isInitialized()) {
      return;
    }

    m_sampleAllocator.recycleSample(m_outputSampleQueuePair);
  }

  OutputSampleType allocateOutputSample(boost::uint32_t sampleIndex) {
    recycleOutputSample();

    return m_sampleAllocator.allocateSample(sampleIndex);
  }

  SampleConverterSharedPtr m_pSampleConverter;
  InputSampleQueuePairType m_inputSampleQueuePair;
  OutputSampleQueuePairType m_outputSampleQueuePair;
  SampleAllocatorType m_sampleAllocator;

};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_PROCESSOR_H
