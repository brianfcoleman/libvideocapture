#ifndef VIDEO_CAPTURE_MESSAGE_SENDER_FACTORY_H
#define VIDEO_CAPTURE_MESSAGE_SENDER_FACTORY_H

#include "boost/shared_ptr.hpp"
#include "boost/thread.hpp"
#include "MessageQueue.hpp"

namespace VideoCapture {

template<typename MessageSender> class MessageSenderFactory {
 public:
  typedef typename MessageSender::MessageType MessageType;
  typedef MessageQueue<MessageType> MessageQueueType;
  typedef typename MessageQueueType::QueueImplType QueueImplType;
  typedef typename MessageQueueType::ImplPtr QueueImplPtr;
  typedef typename MessageQueueType::SizeType SizeType;
  typedef boost::shared_ptr<MessageSender> MessageSenderSharedPtr;
  typedef boost::promise<MessageSenderSharedPtr> MessageSenderFactoryListener;
  typedef boost::shared_ptr<
    MessageSenderFactoryListener> MessageSenderFactoryListenerSharedPtr;
  typedef boost::unique_future<
    MessageSenderSharedPtr> MessageSenderFactoryListenerCallback;

  explicit MessageSenderFactory(
      const MessageSenderFactoryListenerSharedPtr& pListener)
      : m_pListener(pListener) {

  }

  MessageSenderSharedPtr operator()(const MessageQueueType& messageQueue) {
    MessageSenderSharedPtr pMessageSender(new MessageSender(messageQueue));
    std::cout << "Created message sender" << std::endl;
    m_pListener->set_value(pMessageSender);
    return pMessageSender;
  }

 private:
  MessageSenderFactoryListenerSharedPtr m_pListener;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_MESSAGE_SENDER_FACTORY_H
