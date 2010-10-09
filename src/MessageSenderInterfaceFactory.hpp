#ifndef VIDEO_CAPTURE_MESSAGE_SENDER_INTERFACE_FACTORY_H
#define VIDEO_CAPTURE_MESSAGE_SENDER_INTERFACE_FACTORY_H

#include "boost/thread.hpp"

namespace VideoCapture {

template<typename InterfaceType> class MessageSenderInterfaceFactory {
 public:
  typedef typename InterfaceType::MessageSenderFactoryType
      MessageSenderFactoryType;
  typedef typename MessageSenderFactoryType::MessageSenderSharedPtr
      MessageSenderSharedPtr;
  typedef typename MessageSenderFactoryType::MessageSenderFactoryListener
      MessageSenderFactoryListener;
  typedef typename
  MessageSenderFactoryType::MessageSenderFactoryListenerSharedPtr
      MessageSenderFactoryListenerSharedPtr;
  typedef typename
  MessageSenderFactoryType::MessageSenderFactoryListenerCallback
      MessageSenderFactoryListenerCallback;
  typedef typename InterfaceType::MessageReceiverType MessageReceiverType;
  typedef typename MessageReceiverType::MessageReceiverSharedPtr
      MessageReceiverSharedPtr;
  typedef typename MessageReceiverType::SizeType SizeType;

  MessageSenderInterfaceFactory() {

  }

  InterfaceType operator()(SizeType maxSizeMessageQueue) {
    MessageSenderFactoryListenerSharedPtr pListener(
        new MessageSenderFactoryListener());
    MessageSenderFactoryListenerCallback callback(pListener->get_future());
    MessageSenderFactoryType messageSenderFactory(pListener);
    MessageReceiverSharedPtr pMessageReceiver(
        new MessageReceiverType(maxSizeMessageQueue, messageSenderFactory));
    try {
      callback.wait();
      if (callback.has_exception()) {
        InterfaceType messageSenderInterface;
        return messageSenderInterface;
      }
      if (!callback.has_value()) {
        InterfaceType messageSenderInterface;
        return messageSenderInterface;
      }
      MessageSenderSharedPtr pImpl(callback.get());
      if (!pImpl) {
        InterfaceType messageSenderInterface;
        return messageSenderInterface;
      }
      InterfaceType messageSenderInterface(
          pImpl,
          pImpl->messageQueue(),
          pMessageReceiver);
      return messageSenderInterface;
    } catch (boost::future_uninitialized exception) {
      InterfaceType messageSenderInterface;
      return messageSenderInterface;
    } catch (boost::thread_interrupted exception) {
      InterfaceType messageSenderInterface;
      return messageSenderInterface;
    }
  }
};

} // VideoCapture

#endif // VIDEO_CAPTURE_MESSAGE_SENDER_INTERFACE_FACTORY_H
