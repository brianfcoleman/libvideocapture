#include "VideoCaptureDeviceManager.hpp"
#include "VideoCaptureDeviceManagerImpl.hpp"
#include "SynchronousMessage.hpp"
#include "MessageReturnValue.hpp"
#include "boost/function.hpp"
#include "boost/bind.hpp"
#include "PimplUtilities.hpp"

namespace VideoCapture {

VideoCaptureDeviceManager::VideoCaptureDeviceManager() {

}

VideoCaptureDeviceManager::VideoCaptureDeviceManager(
    const VideoCaptureDeviceManager::ImplPtr& pImpl,
    const VideoCaptureDeviceManager::MessageQueueType& messageQueue,
    const VideoCaptureDeviceManager::MessageReceiverSharedPtr& pMessageReceiver)
    : m_pWeakImpl(pImpl),
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
  ImplPtr pImpl(lockImplPtr(m_pWeakImpl));
  if (!pImpl) {
    return messageReturnValue.returnValue();
  }
  MessageProcessorType messageProcessor(
      boost::bind<ReturnType>(
          &VideoCaptureDeviceManagerImpl::videoCaptureDeviceList,
          pImpl));
  sendSynchronousMessage<ReturnType>(
      messageProcessor,
      m_messageQueue,
      messageReturnValue);
  return messageReturnValue.returnValue();
}

bool VideoCaptureDeviceManager::isInitialized() const {
  return VideoCapture::isInitialized(m_pWeakImpl);
}

} // VideoCapture
