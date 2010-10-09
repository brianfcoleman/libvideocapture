#include "SDLVideoManager.hpp"
#include "SDLVideoManagerImpl.hpp"
#include "PimplUtilities.hpp"
#include "MessageReturnValue.hpp"
#include "SynchronousMessage.hpp"
#ifdef DEBUG
#include <iostream>
#endif

namespace VideoCapture {

SDLVideoManager::SDLVideoManager() {

}

SDLVideoManager::SDLVideoManager(
    const ImplPtr& pImpl,
    const MessageQueueType& messageQueue)
    : m_pWeakImpl(pImpl),
      m_messageQueue(messageQueue) {

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
#ifdef DEBUG
  std::cout << "SDLVideoManager::renderToScreen" << std::endl;
#endif
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

bool SDLVideoManager::isInitialized() const {
  return VideoCapture::isInitialized(m_pWeakImpl);
}

} // VideoCapture
