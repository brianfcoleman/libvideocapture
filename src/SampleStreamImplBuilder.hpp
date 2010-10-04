#ifndef VIDEO_CAPTURE_SAMPLE_STREAM_IMPL_BUILDER_H
#define VIDEO_CAPTURE_SAMPLE_STREAM_IMPL_BUILDER_H

#include "SampleStreamImpl.hpp"

namespace VideoCapture {

template<
  typename SampleSource,
  typename SampleSink> class SampleStreamImplBuilder {
 public:
  typedef SampleSource SampleSourceType;
  typedef SampleSink SampleSinkType;
  typedef SampleStreamImpl<SampleSourceType, SampleSinkType> SampleStreamType;
  typedef SampleStreamType ProcessorType;
  typedef typename SampleStreamType::SampleProcessorType SampleProcessorType;
  typedef typename SampleStreamType::SampleProcessorList SampleProcessorList;

  SampleStreamImplBuilder()
      : m_hasSampleSource(false),
        m_hasSampleSink(false) {

  }

  bool addSampleSource(const SampleSourceType& sampleSource) {
    if (m_hasSampleSource) {
      return false;
    }

    m_sampleSource = sampleSource;
    m_hasSampleSource = true;

    return true;
  }

  bool addSampleSink(const SampleSinkType& sampleSink) {
    if (m_hasSampleSink) {
      return false;
    }

    m_sampleSink = sampleSink;
    m_hasSampleSink = true;

    return true;
  }

  bool addSampleProcessor(const SampleProcessorType& sampleProcessor) {
    m_sampleProcessorList.push_back(sampleProcessor);

    return true;
  }

  bool isReadyToBuild() const {
    if (!m_hasSampleSource) {
      return false;
    }

    if (!hasSampleProcessors()) {
      return false;
    }

    if (!m_hasSampleSink) {
      return false;
    }

    return true;
  }

  SampleStreamType build() {
    if (!isReadyToBuild()) {
      SampleStreamType sampleStream;
      return sampleStream;
    }

    SampleStreamType sampleStream(
        m_sampleSource,
        m_sampleSink,
        m_sampleProcessorList);
    return sampleStream;
  }

  operator bool() const {
    return isReadyToBuild();
  }

  SampleStreamType operator()() {
    return build();
  }

 private:
  bool hasSampleProcessors() const {
    if (m_sampleProcessorList.empty()) {
      return false;
    }

    return true;
  }

  SampleSourceType m_sampleSource;
  SampleSinkType m_sampleSink;
  SampleProcessorList m_sampleProcessorList;
  bool m_hasSampleSource;
  bool m_hasSampleSink;

};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_STREAM_IMPL_BUILDER_H
