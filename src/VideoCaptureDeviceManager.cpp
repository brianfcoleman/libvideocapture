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
  if (!isInitialized()) {
    VideoCaptureDeviceList emptyList;
    return emptyList;
  }
  SynchronousMessage<VideoCaptureDeviceList> message(
      boost::bind<VideoCaptureDeviceList>(
          &VideoCaptureDeviceManagerImpl::videoCaptureDeviceList,
          m_pImpl));
  VideoCaptureDeviceList returnValueOnFailure;
  MessageReturnValue<VideoCaptureDeviceList> messageReturnValue(
      message.packagedMessageProcessor(),
      returnValueOnFailure);
  sendSynchronousMessage<VideoCaptureDeviceList>(
      message,
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
