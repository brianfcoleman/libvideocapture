#ifndef VIDEO_CAPTURE_MESSAGE_RETURN_VALUE_H
#define VIDEO_CAPTURE_MESSAGE_RETURN_VALUE_H

namespace VideoCapture {

template<typename ReturnType> class MessageReturnValue {
 public:
  MessageReturnValue(const ReturnType returnValueOnFailure)
      : m_returnValue(returnValueOnFailure),
        m_returnValueOnFailure(returnValueOnFailure),
        m_isInitialized(false) {

  }

  ReturnType returnValue() {
    return m_returnValue;
  }

  void setReturnValue(const ReturnType returnValue) {
    m_returnValue = returnValue;
    m_isInitialized = true;
  }

  void setFailedReturnValue() {
    m_returnValue = m_returnValueOnFailure;
    m_isInitialized = false;
  }

  bool isInitialized() const {
    return m_isInitialized;
  }

  operator bool() const {
    return isInitialized();
  }

 private:
  ReturnType m_returnValue;
  ReturnType m_returnValueOnFailure;
  bool m_isInitialized;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_MESSAGE_RETURN_VALUE_H
