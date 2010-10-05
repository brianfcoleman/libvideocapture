#ifndef VIDEO_CAPTURE_SAMPLE_ALLOCATOR_H
#define VIDEO_CAPTURE_SAMPLE_ALLOCATOR_H

#include <deque>
#include "boost/cstdint.hpp"
#include "boost/shared_ptr.hpp"
#include "RecyclingSampleQueuePair.hpp"

namespace VideoCapture {

template<typename Sample> class SampleAllocator {
 public:
  typedef typename Sample::SampleType SampleType;
  typedef typename SampleType::SampleDataType SampleDataType;
  typedef typename SampleType::SampleFormatType SampleFormatType;
  typedef typename SampleDataType::SampleDataFactory SampleDataFactoryType;
  typedef typename SampleDataFactoryType::SampleDataSharedPtr
  SampleDataSharedPtr;
  typedef typename std::deque<SampleDataSharedPtr> SampleDataContainerType;
  typedef typename SampleDataContainerType::size_type SizeType;
  typedef RecyclingSampleQueuePair<SampleType> SampleQueuePairType;
  typedef typename SampleQueuePairType::SampleQueueSharedPtr
  SampleQueueSharedPtr;

  SampleAllocator() {

  }

  SampleAllocator(
      const SizeType maxCountAllocatedSamples,
      SampleFormatType sampleFormat)
      : m_pSampleDataContainer(
          new SampleDataContainerType()),
        m_sampleFormat(sampleFormat) {
      allocateSampleData(maxCountAllocatedSamples);
  }

  SampleType allocateSample(boost::uint32_t sampleIndex) {
    if (!isInitialized()) {
      SampleType sample;
      return sample;
    }
    if (!canAllocateSample()) {
      SampleType sample;
      return sample;
    }
    SampleDataSharedPtr pSampleData(m_pSampleDataContainer->back());
    m_pSampleDataContainer->pop_back();
#ifdef DEBUG
    std::cout << "SampleAllocator::allocateSample ";
    std::cout << (pSampleData ? "" : "null ") << "sample data";
    std::cout << std::endl;
#endif
    SampleType sample(sampleIndex, m_sampleFormat, pSampleData);
    return sample;
  }

  bool canAllocateSample() {
    if (!isInitialized()) {
      return false;
    }

    if (m_pSampleDataContainer->empty()) {
      return false;
    }

    return true;
  }

  void recycleSample(SampleQueuePairType& sampleQueuePair) {
    if (!isInitialized()) {
      return;
    }

    if (canAllocateSample()) {
      return;
    }

    SampleQueueSharedPtr pRecycledSampleQueue(
        sampleQueuePair.recycledSampleQueue());
    if (!pRecycledSampleQueue) {
      return;
    }
    SampleType recycledSample(
        pRecycledSampleQueue->removeSample());
    if (!recycledSample) {
      return;
    }

    recycleSample(recycledSample);
  }

  void recycleSample(SampleType& sample) {
    if (!isInitialized()) {
      return;
    }

    if (!sample) {
      return;
    }

    SampleDataSharedPtr pSampleData(sample.sampleData());
    if (!pSampleData) {
      return;
    }

    m_pSampleDataContainer->push_front(pSampleData);
  }

  SampleFormatType sampleFormat() const {
    return m_sampleFormat;
  }

  bool isInitialized() const {
    if (!m_sampleFormat) {
      return false;
    }

    return true;
  }

  operator bool() const {
    return isInitialized();
  }

 private:
  boost::shared_ptr<SampleDataContainerType> m_pSampleDataContainer;
  SampleFormatType m_sampleFormat;
  void allocateSampleData(SizeType maxCountAllocatedSamples) {
    if (!isInitialized()) {
      return;
    }
    SampleDataFactoryType sampleDataFactory;
    for (SizeType i = 0; i < maxCountAllocatedSamples; ++i) {
      SampleDataSharedPtr pSampleData(
          sampleDataFactory(m_sampleFormat));
      m_pSampleDataContainer->push_front(pSampleData);
#ifdef DEBUG
    std::cout << "SampleAllocator::allocateSampleData ";
    std::cout << (pSampleData ? "" : "null ") << "sample data ";
    std::cout << pSampleData.get() << std::endl;
    std::cout << "size " << m_pSampleDataContainer->size() << std::endl;
#endif
    }
  }
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_ALLOCATOR_H
