#include "SDLVideoManager.hpp"
#include "SDLVideoManagerImpl.hpp"
#include "PimplUtilities.hpp"
#include "MessageReturnValue.hpp"
#include "SynchronousMessage.hpp"
#include "SDLNullEventProcessorCallback.hpp"

namespace VideoCapture {

SDLVideoManager::SDLVideoManager() {

}

SDLVideoManager::SDLVideoManager(
    const ImplPtr& pImpl,
    const MessageQueueType& messageQueue,
    const MessageReceiverSharedPtr& pMessageReceiver)
    : m_pWeakImpl(pImpl),
      m_messageQueue(messageQueue),
      m_pMessageReceiver(pMessageReceiver) {
}

bool SDLVideoManager::setVideoMode(const RGBVideoFormat& videoFormat) {
  typedef bool ReturnType;
  typedef boost::function<ReturnType (void)> MessageProcessorType;
  typedef MessageReturnValue<ReturnType> MessageReturnValueType;
  MessageReturnValueType messageReturnValue(false);
  if (!isInitialized()) {
    return messageReturnValue.returnValue();
  }
  ImplPtr pImpl(lockImplPtr(m_pWeakImpl));
  if (!pImpl) {
    return messageReturnValue.returnValue();
  }
  MessageProcessorType messageProcessor(
      boost::bind<ReturnType>(
          &SDLManagerImpl::setVideoMode,
          pImpl,
          videoFormat));
  sendSynchronousMessage<ReturnType>(
      messageProcessor,
      m_messageQueue,
      messageReturnValue);
  return messageReturnValue.returnValue();
}

bool SDLVideoManager::renderToScreen(
    SDLVideoManager::RGBVideoSample& videoSample) {
  typedef bool ReturnType;
  typedef boost::function<ReturnType (void)> MessageProcessorType;
  typedef MessageReturnValue<ReturnType> MessageReturnValueType;
  MessageReturnValueType messageReturnValue(false);
  if (!isInitialized()) {
    return messageReturnValue.returnValue();
  }
  ImplPtr pImpl(lockImplPtr(m_pWeakImpl));
  if (!pImpl) {
    return messageReturnValue.returnValue();
  }
  MessageProcessorType messageProcessor(
      boost::bind<ReturnType>(
          &SDLManagerImpl::renderToScreen,
          pImpl,
          videoSample));
  sendSynchronousMessage<ReturnType>(
      messageProcessor,
      m_messageQueue,
      messageReturnValue);
  return messageReturnValue.returnValue();
}

bool SDLVideoManager::startEventProcessor() {
  typedef bool ReturnType;
  typedef boost::function<ReturnType (void)> MessageProcessorType;
  typedef MessageReturnValue<ReturnType> MessageReturnValueType;
  MessageReturnValueType messageReturnValue(false);
  if (!isInitialized()) {
    return messageReturnValue.returnValue();
  }
  ImplPtr pImpl(lockImplPtr(m_pWeakImpl));
  if (!pImpl) {
    return messageReturnValue.returnValue();
  }
  SDLNullEventProcessorCallback nullCallback;
  MessageProcessorType messageProcessor(
      boost::bind<ReturnType>(
          &SDLManagerImpl::startEventProcessor,
          pImpl,
          nullCallback));
  sendSynchronousMessage<ReturnType>(
      messageProcessor,
      m_messageQueue,
      messageReturnValue);
  return messageReturnValue.returnValue();
}

bool SDLVideoManager::waitForQuitEvent() {
  typedef bool ReturnType;
  typedef boost::function<ReturnType (void)> MessageProcessorType;
  typedef MessageReturnValue<ReturnType> MessageReturnValueType;
  MessageReturnValueType messageReturnValue(false);
  if (!isInitialized()) {
    return messageReturnValue.returnValue();
  }
  ImplPtr pImpl(lockImplPtr(m_pWeakImpl));
  if (!pImpl) {
    return messageReturnValue.returnValue();
  }

  MessageProcessorType messageProcessor(
      boost::bind<ReturnType>(
          &SDLManagerImpl::waitForQuitEvent,
          pImpl));
  sendSynchronousMessage<ReturnType>(
      messageProcessor,
      m_messageQueue,
      messageReturnValue);
  return messageReturnValue.returnValue();
}

bool SDLVideoManager::isInitialized() const {
  return VideoCapture::isInitialized(m_pWeakImpl);
}



} // VideoCapture
