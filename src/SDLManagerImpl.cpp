#include "SDLManagerImpl.hpp"

namespace VideoCapture {

SDLManagerImpl::SDLManagerImpl(
    Uint32 flags,
    const SDLManagerImpl::MessageQueueType& messageQueue)
    : m_isInitialized(false),
      m_messageQueue(messageQueue) {
  m_isInitialized = initialize(flags);
}

SDLManagerImpl::~SDLManagerImpl() {
  if (!isInitialized()) {
    return;
  }
  SDL_Quit();
}

SDLManagerImpl::MessageQueueType SDLManagerImpl::messageQueue() {
  return m_messageQueue;
}

bool SDLManagerImpl::initialize(Uint32 flags) {
  int result = SDL_Init(flags);
  if (result < 0) {
    return false;
  }

  return true;
}

bool SDLManagerImpl::isInitialized() const {
  return m_isInitialized;
}

bool SDLManagerImpl::setVideoMode(const RGBVideoFormat& videoFormat) {
  if (!videoFormat) {
    return false;
  }
  if (!isInitialized()) {
    return false;
  }
  SDLRendererSharedPtr pRenderer(new SDLRenderer(videoFormat));
  if (!pRenderer) {
    return false;
  }
  if (!pRenderer->isInitialized()) {
    return false;
  }
  m_pRenderer = pRenderer;
  return true;
}

bool SDLManagerImpl::renderToScreen(RGBVideoSample& videoSample) {
  if (!videoSample) {
    return false;
  }
  if (!isInitialized()) {
    return false;
  }
  if (!m_pRenderer) {
    return false;
  }
  if (!m_pRenderer->isInitialized()) {
    return false;
  }
  if (!m_pRenderer->renderToScreen(videoSample)) {
    return false;
  }
  return true;
}

bool SDLManagerImpl::startEventProcessor(
    const EventProcessorCallback& eventProcessorCallback) {
  if (!isInitialized()) {
    return false;
  }
  if (m_pEventProcessor) {
    return true;
  }
  SDLEventProcessorSharedPtr pEventProcessor(
      new SDLEventProcessor(eventProcessorCallback));
  if (!pEventProcessor) {
    return false;
  }
  m_pEventProcessor = pEventProcessor;
  return true;
}

bool SDLManagerImpl::waitForQuitEvent() {
  if (!isInitialized()) {
    return false;
  }
  if (!m_pEventProcessor) {
    return false;
  }
  m_pEventProcessor->joinThread();
  return true;
}

} // VideoCapture
