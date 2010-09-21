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
  typedef boost::function<ReturnType ()> MessageProcessorType;
  typedef MessageReturnValue<ReturnType> MessageReturnValueType;
  typedef boost::packaged_task<ReturnType> PackagedTaskType;
  typedef boost::shared_ptr<PackagedTaskType> PackagedTaskSharedPtr;

  SynchronousMessage(const MessageProcessorType& messageProcessor)
      : Message(),
        m_pPackagedMessageProcessor(new PackagedTaskType(messageProcessor)) {

  }

  void processMessage() {
    if (!isInitialized()) {
      return;
    }
    m_pPackagedMessageProcessor->operator()();
  }

  PackagedTaskSharedPtr packagedMessageProcessor() {
    return m_pPackagedMessageProcessor;
  }

  bool isInitialized() const {
    if (!m_pPackagedMessageProcessor) {
      return false;
    }

    return true;
  }

  operator bool() const {
    return isInitialized();
  }

 private:
  PackagedTaskSharedPtr m_pPackagedMessageProcessor;

};

template<typename ReturnType> void sendSynchronousMessage(
    SynchronousMessage<ReturnType>& message,
    MessageQueue<Message>& messageQueue,
    MessageReturnValue<ReturnType>& messageReturnValue) {
  messageQueue.addMessage(message);
  messageReturnValue.waitForMessageReturnValue();
}

} // VideoCapture

#endif // VIDEO_CAPTURE_SYNCHRONOUS_MESSAGE_H
