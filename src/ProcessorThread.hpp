#ifndef VIDEO_CAPTURE_PROCESSR_THREAD_H
#define VIDEO_CAPTURE_PROCESSR_THREAD_H

#include "boost/utility.hpp"
#include "boost/thread.hpp"
#include "boost/function.hpp"
#include "boost/bind.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"

namespace VideoCapture {

template<typename ProcessorFactory> class ProcessorThread {
 public:
  typedef ProcessorFactory ProcessorFactoryType;
  typedef typename ProcessorFactoryType::ProcessorType ProcessorType;

  explicit ProcessorThread(const ProcessorFactoryType& processorFactory)
      : m_pThreadEndIndicator(new boost::promise<bool>()),
        m_thread(
            boost::bind(
                &processorLoop<ProcessorFactoryType>,
                processorFactory,
                m_pThreadEndIndicator)) {

  }

  ~ProcessorThread() {
    stopProcessing();
  }

 private:
  void stopProcessing() {
    if (!m_pThreadEndIndicator) {
      return;
    }
    m_pThreadEndIndicator->set_value(true);
  }

  boost::shared_ptr<boost::promise<bool>> m_pThreadEndIndicator;
  boost::thread m_thread;

};

template<typename ProcessorFactory> void processorLoop(
    ProcessorFactory& processorFactory,
    boost::weak_ptr<boost::promise<bool>> pWeakThreadEndIndicator) {
  typedef ProcessorFactory ProcessorFactoryType;
  typedef typename ProcessorFactoryType::ProcessorType ProcessorType;

  try {
    boost::unique_future<bool> isThreadEnded;
    {
      boost::shared_ptr<boost::promise<bool>> pThreadEndIndicator(
          pWeakThreadEndIndicator.lock());
      if (!pThreadEndIndicator) {
        return;
      }
      isThreadEnded.swap(pThreadEndIndicator->get_future());
    }
    ProcessorType processor(processorFactory());
    while (!boost::this_thread::interruption_requested()) {
      if (isThreadEnded.is_ready()) {
        return;
      }
      processor();
      boost::this_thread::interruption_point();
    }
  } catch (boost::thread_interrupted exception) {
#ifdef DEBUG
    std::cout << "ProcessorThread::processorLoop exiting" << std::endl;
#endif
    return;
  }
#ifdef DEBUG
  std::cout << "ProcessorThread::processorLoop exiting" << std::endl;
#endif
}

} // VideoCapture

#endif // VIDEO_CAPTURE_PROCESSR_THREAD_H
