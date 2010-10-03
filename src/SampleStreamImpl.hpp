#ifndef VIDEO_CAPTURE_SAMPLE_STREAM_IMPL_H
#define VIDEO_CAPTURE_SAMPLE_STREAM_IMPL_H

#include <list>
#include "boost/function.hpp"

namespace VideoCapture {

template<typename SampleSource, typename SampleSink> class SampleStreamImpl {
 public:
  typedef SampleSource SampleSourceType;
  typedef SampleSink SampleSinkType;
  typedef boost::function<void ()> SampleProcessorType;
  typedef std::list<SampleProcessorType> SampleProcessorList;
  typedef SampleProcessorList::iterator SampleProcessorIterator;

  SampleStreamImpl() {

  }

  SampleStreamImpl(
      const SampleSourceType& sampleSource,
      const SampleSinkType& sampleSink,
      const SampleProcessorList& sampleProcessorList)
      : m_sampleSource(sampleSource),
        m_sampleSink(sampleSink),
        m_sampleProcessorList(sampleProcessorList) {

  }

  void processSampleStream() {
    if (m_sampleProcessorList.empty()) {
      return;
    }
    for (SampleProcessorIterator iterator(m_sampleProcessorList.begin());
         iterator != m_sampleProcessorList.end();
         ++iterator) {
      SampleProcessorType sampleProcessor(*iterator);
      sampleProcessor();
    }
  }

  void operator()() {
    processSampleStream();
  }

 private:
  SampleSourceType m_sampleSource;
  SampleSinkType m_sampleSink;
  SampleProcessorList m_sampleProcessorList;

};

} // VideoCapture
#endif // VIDEO_CAPTURE_SAMPLE_STREAM_IMPL_H
