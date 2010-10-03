#ifndef VIDEO_CAPTURE_MESSAGE_RECEIVER_H
#define VIDEO_CAPTURE_MESSAGE_RECEIVER_H

#include "boost/utility.hpp"
#include "boost/thread.hpp"
#include "boost/function.hpp"
#include "boost/bind.hpp"
#include "MessageQueue.hpp"

namespace VideoCapture {

template<
  typename MessageSenderFactory> class MessageReceiver : boost::noncopyable {
 public:
  typedef typename MessageSenderFactory::MessageType MessageType;
  typedef typename MessageSenderFactory::MessageSharedPtr MessageSharedPtr;
  typedef MessageQueue<MessageType> MessageQueueType;
  typedef typename MessageQueueType::QueueImplType QueueImplType;
  typedef typename MessageQueueType::ImplPtr QueueImplPtr;
  typedef typename MessageQueueType::SizeType SizeType;

  MessageReceiver(
      const SizeType maxSizeMessageQueue,
      const MessageSenderFactory& messageSenderFactory)
      : m_pMessageQueue(new QueueImplType(maxSizeMessageQueue)),
        m_thread(
            boost::bind(
                &receiveMessageLoop<MessageSenderFactory>,
                m_pMessageQueue,
                messageSenderFactory)) {

  }

  ~MessageReceiver() {
    stopReceivingMessages();
  }

 private:
  void stopReceivingMessages() {
    m_thread.interrupt();
  }

  QueueImplPtr m_pMessageQueue;
  boost::thread m_thread;
};

template<typename MessageSenderFactory> void receiveMessageLoop(
    typename MessageSenderFactory::QueueImplPtr pMessageQueue,
    MessageSenderFactory& messageSenderFactory) {
  typedef typename MessageSenderFactory::MessageType MessageType;
  typedef typename MessageSenderFactory::MessageSharedPtr MessageSharedPtr;
  typedef typename MessageSenderFactory::MessageQueueType MessageQueueType;
  typedef typename
      MessageSenderFactory::MessageSenderSharedPtr MessageSenderSharedPtr;

  try {
    MessageQueueType messageQueue(pMessageQueue);
    MessageSenderSharedPtr pMessageSender(messageSenderFactory(messageQueue));
    while (!boost::this_thread::interruption_requested()) {
      MessageSharedPtr pMessage(pMessageQueue->pop_back());
      if (!pMessage) {
        continue;
      }
      pMessage->operator()();
    }
  } catch (boost::thread_interrupted exception) {

  }
}

} // VideoCapture

#endif // VIDEO_CAPTURE_MESSAGE_RECEIVER_H
