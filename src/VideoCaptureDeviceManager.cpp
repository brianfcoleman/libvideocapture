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

VideoCaptureDeviceManager::VideoCaptureDeviceList
VideoCaptureDeviceManager::videoCaptureDeviceList() const {
  typedef VideoCaptureDeviceList ReturnType;
  typedef boost::function<ReturnType (void)> MessageProcessorType;
  typedef MessageReturnValue<ReturnType> MessageReturnValueType;
  ReturnType emptyList;
  MessageReturnValueType messageReturnValue(emptyList);
  if (!isInitialized()) {
    return messageReturnValue.returnValue();
  }
  MessageProcessorType messageProcessor(
      boost::bind<ReturnType>(
          &VideoCaptureDeviceManagerImpl::videoCaptureDeviceList,
          m_pImpl));
  sendSynchronousMessage<ReturnType>(
      messageProcessor,
      m_messageQueue,
      messageReturnValue);
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
