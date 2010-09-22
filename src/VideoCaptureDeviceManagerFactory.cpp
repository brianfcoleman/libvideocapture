#include "VideoCaptureDeviceManagerFactory.hpp"
#include "VideoCaptureDeviceManagerImpl.hpp"

namespace VideoCapture {

VideoCaptureDeviceManagerFactory::VideoCaptureDeviceManagerFactory() {

}


VideoCaptureDeviceManager
VideoCaptureDeviceManagerFactory::createVideoCaptureDeviceManager() {
  typedef VideoCaptureDeviceManager::MessageSenderFactoryType
      MessageSenderFactoryType;
  typedef MessageSenderFactoryType::MessageSenderSharedPtr
      MessageSenderSharedPtr;
  typedef MessageSenderFactoryType::MessageSenderFactoryListener
      MessageSenderFactoryListener;
  typedef MessageSenderFactoryType::MessageSenderFactoryListenerSharedPtr
      MessageSenderFactoryListenerSharedPtr;
  typedef MessageSenderFactoryType::MessageSenderFactoryListenerCallback
      MessageSenderFactoryListenerCallback;
  typedef VideoCaptureDeviceManager::MessageReceiverType MessageReceiverType;
  typedef VideoCaptureDeviceManager::MessageReceiverSharedPtr
      MessageReceiverSharedPtr;
  MessageSenderFactoryListenerSharedPtr pListener(
      new MessageSenderFactoryListener());
  MessageSenderFactoryListenerCallback callback(pListener->get_future());
  MessageSenderFactoryType messageSenderFactory(pListener);
  MessageReceiverSharedPtr pMessageReceiver(
      new MessageReceiverType(s_kMaxSizeMessageQueue, messageSenderFactory));
  try {
    callback.wait();
    if (callback.has_exception()) {
      VideoCaptureDeviceManager videoCaptureDeviceManager;
      return videoCaptureDeviceManager;
    }
    if (!callback.has_value()) {
      VideoCaptureDeviceManager videoCaptureDeviceManager;
      return videoCaptureDeviceManager;
    }
    MessageSenderSharedPtr pImpl(callback.get());
    if (!pImpl) {
      VideoCaptureDeviceManager videoCaptureDeviceManager;
      return videoCaptureDeviceManager;
    }
    VideoCaptureDeviceManager videoCaptureDeviceManager(
        pImpl,
        pImpl->messageQueue(),
        pMessageReceiver);
    return videoCaptureDeviceManager;
  } catch (boost::future_uninitialized exception) {
    VideoCaptureDeviceManager videoCaptureDeviceManager;
    return videoCaptureDeviceManager;
  } catch (boost::thread_interrupted exception) {
    VideoCaptureDeviceManager videoCaptureDeviceManager;
    return videoCaptureDeviceManager;
  }
}


} // VideoCapture