#ifndef VIDEO_CAPTURE_MESSAGE_RECEIVER_H
#define VIDEO_CAPTURE_MESSAGE_RECEIVER_H

#include "boost/utility.hpp"
#include "boost/thread.hpp"
#include "boost/function.hpp"
#include "boost/bind.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"
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
  typedef MessageReceiver<MessageSenderFactory> MessageReceiverType;
  typedef boost::shared_ptr<MessageReceiverType> MessageReceiverSharedPtr;

  MessageReceiver(
      const SizeType maxSizeMessageQueue,
      const MessageSenderFactory& messageSenderFactory)
      : m_pThreadEndIndicator(new boost::promise<bool>()),
        m_pMessageQueue(new QueueImplType(maxSizeMessageQueue)),
        m_thread(
            boost::bind(
                &receiveMessageLoop<MessageSenderFactory>,
                m_pMessageQueue,
                messageSenderFactory,
                m_pThreadEndIndicator)) {

  }

  ~MessageReceiver() {
#ifdef DEBUG
    std::cout << "MessageReceiver::~MessageReceiver exiting" << std::endl;
#endif
    stopReceivingMessages();
  }

 private:
  void stopReceivingMessages() {
    if (!m_pThreadEndIndicator) {
      return;
    }
    m_pThreadEndIndicator->set_value(true);
  }

  boost::shared_ptr<boost::promise<bool>> m_pThreadEndIndicator;
  QueueImplPtr m_pMessageQueue;
  boost::thread m_thread;
};

template<typename MessageSenderFactory> void receiveMessageLoop(
    typename MessageSenderFactory::QueueImplPtr pMessageQueue,
    MessageSenderFactory& messageSenderFactory,
    boost::weak_ptr<boost::promise<bool>> pWeakThreadEndIndicator) {
  typedef typename MessageSenderFactory::MessageType MessageType;
  typedef typename MessageSenderFactory::MessageSharedPtr MessageSharedPtr;
  typedef typename MessageSenderFactory::MessageQueueType MessageQueueType;
  typedef typename
      MessageSenderFactory::MessageSenderSharedPtr MessageSenderSharedPtr;

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
    MessageQueueType messageQueue(pMessageQueue);
    MessageSenderSharedPtr pMessageSender(messageSenderFactory(messageQueue));
    while (!boost::this_thread::interruption_requested()) {
      if (isThreadEnded.is_ready()) {
        return;
      }
      MessageSharedPtr pMessage(pMessageQueue->pop_back());
      if (!pMessage) {
        continue;
      }
      pMessage->operator()();
      boost::this_thread::interruption_point();
    }
  } catch (boost::thread_interrupted exception) {
#ifdef DEBUG
    std::cout << "MessageReceiver::receiveMessageLoop exiting" << std::endl;
#endif
    return;
  }
#ifdef DEBUG
    std::cout << "MessageReceiver::receiveMessageLoop exiting" << std::endl;
#endif
}

} // VideoCapture

#endif // VIDEO_CAPTURE_MESSAGE_RECEIVER_H
