#ifndef VIDEO_CAPTURE_PROCESSR_THREAD_H
#define VIDEO_CAPTURE_PROCESSR_THREAD_H

#include "boost/utility.hpp"
#include "boost/thread.hpp"
#include "boost/function.hpp"
#include "boost/bind.hpp"

namespace VideoCapture {

template<typename ProcessorFactory> class ProcessorThread {
 public:
  typedef ProcessorFactory ProcessorFactoryType;
  typedef typename ProcessorFactoryType::ProcessorType ProcessorType;

  explicit ProcessorThread(const ProcessorFactoryType& processorFactory)
      : m_thread(
          boost::bind(&processorLoop<ProcessorFactoryType>, processorFactory)) {

  }

  ~ProcessorThread() {
    stopProcessing();
  }

 private:
  void stopProcessing() {
    m_thread.interrupt();
  }

  boost::thread m_thread;

};

template<typename ProcessorFactory> void processorLoop(
    ProcessorFactory& processorFactory) {
  typedef ProcessorFactory ProcessorFactoryType;
  typedef typename ProcessorFactoryType::ProcessorType ProcessorType;

  try {
    ProcessorType processor(processorFactory());
    while (!boost::this_thread::interruption_requested()) {
      processor();
    }
  } catch (boost::thread_interrupted exception) {

  }
}

} // VideoCapture

#endif // VIDEO_CAPTURE_PROCESSR_THREAD_H
