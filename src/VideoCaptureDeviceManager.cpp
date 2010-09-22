#include "VideoCaptureDeviceManager.hpp"
#include "VideoCaptureDeviceManagerImpl.hpp"
#include "SynchronousMessage.hpp"
#include "MessageReturnValue.hpp"
#include "boost/function.hpp"
#include "boost/bind.hpp"

namespace VideoCapture {

VideoCaptureDeviceManager::VideoCaptureDeviceManager() {

}

VideoCaptureDeviceManager::VideoCaptureDeviceManager(
    const VideoCaptureDeviceManager::ImplPtr& pImpl,
    const VideoCaptureDeviceManager::MessageQueueType& messageQueue,
    const VideoCaptureDeviceManager::MessageReceiverSharedPtr& pMessageReceiver)
    : m_pImpl(pImpl),
      m_messageQueue(messageQueue),
      m_pMessageReceiver(pMessageReceiver) {

}

const VideoCaptureDeviceManager::VideoCaptureDeviceList
VideoCaptureDeviceManager::videoCaptureDeviceList() const {
  typedef SynchronousMessage<VideoCaptureDeviceList> SynchronousMessageType;
  typedef SynchronousMessageType::SynchronousMessageSharedPtr
      SynchronousMessageSharedPtr;
  if (!isInitialized()) {
    VideoCaptureDeviceList emptyList;
    return emptyList;
  }
  SynchronousMessageSharedPtr pMessage(
      new SynchronousMessageType(
          boost::bind<VideoCaptureDeviceList>(
              &VideoCaptureDeviceManagerImpl::videoCaptureDeviceList,
              m_pImpl)));
  VideoCaptureDeviceList returnValueOnFailure;
  MessageReturnValue<VideoCaptureDeviceList> messageReturnValue(
      returnValueOnFailure);
  sendSynchronousMessage<VideoCaptureDeviceList>(
      pMessage,
      m_messageQueue,
      messageReturnValue);
  if (!messageReturnValue) {
    VideoCaptureDeviceList emptyList;
    return emptyList;
  }
  return messageReturnValue.returnValue();
}

bool VideoCaptureDeviceManager::isInitialized() const {
  if (!m_pImpl) {
    return false;
  }
  if (!m_pImpl->isInitialized()) {
    return false;
  }

  return true;
}

} // VideoCapture
