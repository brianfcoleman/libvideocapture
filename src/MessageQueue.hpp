#ifndef VIDEO_CAPTURE_MESSAGE_QUEUE_H
#define VIDEO_CAPTURE_MESSAGE_QUEUE_H

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "PimplUtilities.hpp"
#include "BoundedBuffer.hpp"

namespace VideoCapture {

template<typename MessageType> class MessageQueue {
 public:
  typedef boost::shared_ptr<MessageType> MessageSharedPtr;
  typedef bounded_buffer<MessageSharedPtr> QueueImplType;
  typedef typename QueueImplType::size_type SizeType;
  typedef boost::shared_ptr<QueueImplType> ImplPtr;
  typedef boost::weak_ptr<QueueImplType> ImplWeakPtr;

  MessageQueue() {

  }

  explicit MessageQueue(const ImplPtr& pImpl)
      : m_pWeakImpl(pImpl) {

  }

  bool addMessage(const MessageSharedPtr& pMessage) {
    ImplPtr pImpl(lockImplPtr(m_pWeakImpl));
    if (!pImpl) {
      return false;
    }
    pImpl->push_front(pMessage);

    return true;
  }

  MessageSharedPtr removeMessage() {
    ImplPtr pImpl(lockImplPtr(m_pWeakImpl));
    if (!pImpl) {
      MessageSharedPtr pEmptyMessage;
      return pEmptyMessage;
    }
    return pImpl->pop_back();
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
