#ifndef VIDEO_CAPTURE_SAMPLE_SINK_H
#define VIDEO_CAPTURE_SAMPLE_SINK_H

#include "RecyclingSampleQueuePair.hpp"

namespace VideoCapture {

template<typename Sample> class SampleSink {
 public:
  typedef Sample SampleType;
  typedef RecyclingSampleQueuePair<SampleType> SampleQueuePairType;
  typedef typename SampleQueuePairType::SampleQueueSharedPtr
  SampleQueueSharedPtr;

  SampleSink() {

  }

  SampleSink(const SampleQueuePairType& sampleQueuePair)
      : m_sampleQueuePair(sampleQueuePair) {

  }

  SampleType removeSample() {
    SampleQueueSharedPtr pSampleQueue(m_sampleQueuePair.sampleQueue());
    if (!pSampleQueue) {
      SampleType sample;
      return sample;
    }

    return pSampleQueue->removeSample();
  }

  void recycleSample(const SampleType& sample) {
    if (!sample) {
      return;
    }

    SampleQueueSharedPtr pRecycledSampleQueue(
        m_sampleQueuePair.recycledSampleQueue());
    if (!pRecycledSampleQueue) {
      return;
    }

    pRecycledSampleQueue->addSample(sample);
  }

 private:
  SampleQueuePairType m_sampleQueuePair;

};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_SINK_H
