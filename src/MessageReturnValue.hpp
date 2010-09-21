#ifndef VIDEO_CAPTURE_MESSAGE_RETURN_VALUE_H
#define VIDEO_CAPTURE_MESSAGE_RETURN_VALUE_H

#include "boost/thread.hpp"

namespace VideoCapture {

template<typename ReturnType> class MessageReturnValue {
 public:
  typedef boost::packaged_task<ReturnType> PackagedTaskType;
  typedef boost::shared_ptr<PackagedTaskType> PackagedTaskSharedPtr;
  typedef boost::unique_future<ReturnType> UniqueFutureType;

  MessageReturnValue(
      const PackagedTaskSharedPtr& pPackagedMessageProcessor,
      const ReturnType returnValueOnFailure)
      : m_futureReturnValue(pPackagedMessageProcessor->get_future()),
        m_returnValue(returnValueOnFailure),
        m_returnValueOnFailure(returnValueOnFailure),
        m_isInitialized(false) {

  }

  void waitForMessageReturnValue() {
    try {
      m_futureReturnValue.wait();
      if (m_futureReturnValue.has_exception()) {
        setFailedReturnValue();
        return;
      }
      if (!m_futureReturnValue.has_value()) {
        setFailedReturnValue();
        return;
      }
      setReturnValue(m_futureReturnValue.get());
    } catch (boost::future_uninitialized exception) {
      setFailedReturnValue();
    } catch (boost::thread_interrupted exception) {
      setFailedReturnValue();
    }
  }

  ReturnType returnValue() {
    return m_returnValue;
  }

  bool isInitialized() const {
    return m_isInitialized;
  }

  operator bool() const {
    return isInitialized();
  }

 private:
  void setReturnValue(const ReturnType returnValue) {
    m_returnValue = returnValue;
    m_isInitialized = true;
  }

  void setFailedReturnValue() {
    m_returnValue = m_returnValueOnFailure;
    m_isInitialized = false;
  }

  UniqueFutureType m_futureReturnValue;
  ReturnType m_returnValue;
  ReturnType m_returnValueOnFailure;
  bool m_isInitialized;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_MESSAGE_RETURN_VALUE_H
