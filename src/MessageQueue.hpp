#ifndef VIDEO_CAPTURE_MESSAGE_QUEUE_H
#define VIDEO_CAPTURE_MESSAGE_QUEUE_H

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "PimplUtilities.hpp"
#include "BoundedBuffer.hpp"

namespace VideoCapture {

template<typename MessageType> class MessageQueue {
 public:
  typedef bounded_buffer<MessageType> QueueImplType;
  typedef typename QueueImplType::size_type SizeType;
  typedef boost::shared_ptr<QueueImplType> ImplPtr;
  typedef boost::weak_ptr<QueueImplType> ImplWeakPtr;

  MessageQueue() {

  }

  explicit MessageQueue(const ImplPtr& pImpl)
      : m_pWeakImpl(pImpl) {

  }

  bool addMessage(const MessageType& message) {
    ImplPtr pImpl(lockImplPtr(m_pWeakImpl));
    if (!pImpl) {
      return false;
    }
    pImpl->push_front(message);

    return true;
  }

  MessageType removeMessage() {
    ImplPtr pImpl(lockImplPtr(m_pWeakImpl));
    if (!pImpl) {
      MessageType emptyMessage;
      return emptyMessage;
    }
    return pImpl->pop_back(message);
  }

  bool isInitialized() const {
    return isInitialized(m_pWeakImpl);
  }

  operator bool() const {
    return isInitialized();
  }

 private:
  ImplWeakPtr m_pWeakImpl;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_MESSAGE_QUEUE_H
