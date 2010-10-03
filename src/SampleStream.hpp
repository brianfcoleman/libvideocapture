#ifndef VIDEO_CAPTURE_SAMPLE_STREAM_H
#define VIDEO_CAPTURE_SAMPLE_STREAM_H

#include "boost/shared_ptr.hpp"
#include "SampleStreamImplBuilder.hpp"
#include "ProcessorThread.hpp"

namespace VideoCapture {

template<typename SampleSource, typename SampleSink> class SampleStream {
 public:
  typedef SampleSource SampleSourceType;
  typedef SampleSink SampleSinkType;
  typedef SampleStreamImplBuilder<
    SampleSourceType, SampleSinkType> SampleStreamBuilderType;
  typedef ProcessorThread<SampleStreamBuilderType> ProcessorThreadType;
  typedef boost::shared_ptr<ProcessorThreadType> ProcessorThreadSharedPtr;

  SampleStream() {

  }

  explicit SampleStream(ProcessorThreadSharedPtr pProcessorThread)
      : m_pProcessorThread(pProcessorThread) {

  }

  bool isInitialized() const {
    if (!m_pProcessorThread) {
      return false;
    }

    return true;
  }

  operator bool() const {
    return isInitialized();
  }

 private:
  ProcessorThreadSharedPtr m_pProcessorThread;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_STREAM_H
