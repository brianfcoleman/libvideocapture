#ifndef VIDEO_CAPTURE_SYNCHRONOUS_MESSAGE_H
#define VIDEO_CAPTURE_SYNCHRONOUS_MESSAGE_H

#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/thread.hpp"
#include "Message.hpp"
#include "MessageQueue.hpp"
#include "MessageReturnValue.hpp"

namespace VideoCapture {

template<typename ReturnType> class SynchronousMessage : public Message {
 public:
  typedef SynchronousMessage<ReturnType> SynchronousMessageType;
  typedef boost::shared_ptr<SynchronousMessageType> SynchronousMessageSharedPtr;
  typedef boost::function<ReturnType ()> MessageProcessorType;
  typedef MessageReturnValue<ReturnType> MessageReturnValueType;
  typedef boost::packaged_task<ReturnType> PackagedTaskType;
  typedef boost::unique_future<ReturnType> UniqueFutureType;

  SynchronousMessage(const MessageProcessorType& messageProcessor)
      : Message(),
        m_packagedMessageProcessor(messageProcessor),
        m_futureReturnValue(m_packagedMessageProcessor.get_future()) {

  }

  void processMessage() {
    m_packagedMessageProcessor();
  }

  void waitForMessageReturnValue(MessageReturnValueType& messageReturnValue) {
    try {
      m_futureReturnValue.wait();
      if (m_futureReturnValue.has_exception()) {
        messageReturnValue.setFailedReturnValue();
        return;
      }
      if (!m_futureReturnValue.has_value()) {
        messageReturnValue.setFailedReturnValue();
        return;
      }
      messageReturnValue.setReturnValue(m_futureReturnValue.get());
    } catch (boost::future_uninitialized exception) {
      messageReturnValue.setFailedReturnValue();
    } catch (boost::thread_interrupted exception) {
      messageReturnValue.setFailedReturnValue();
    }
  }

 private:
  PackagedTaskType m_packagedMessageProcessor;
  UniqueFutureType m_futureReturnValue;
};

template<typename ReturnType> void sendSynchronousMessage(
    boost::function<ReturnType ()> messageProcessor,
    MessageQueue<Message>& messageQueue,
    MessageReturnValue<ReturnType>& messageReturnValue) {
  typedef SynchronousMessage<ReturnType> SynchronousMessageType;
  typedef SynchronousMessageType::SynchronousMessageSharedPtr
      SynchronousMessageSharedPtr;
  SynchronousMessageSharedPtr pMessage(
      new SynchronousMessageType(messageProcessor));
  messageQueue.addMessage(pMessage);
  pMessage->waitForMessageReturnValue(messageReturnValue);
}

} // VideoCapture

#endif // VIDEO_CAPTURE_SYNCHRONOUS_MESSAGE_H
