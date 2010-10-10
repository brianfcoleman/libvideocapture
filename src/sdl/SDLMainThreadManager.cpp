#include "SDLMainThreadManager.hpp"
#include "SDLVideoManagerImpl.hpp"

namespace VideoCapture {

SDLMainThreadManager::SDLMainThreadManager()
    : m_pImpl(new SDLVideoManagerImpl()),
      m_pQueueImpl(new MessageQueueImplType(s_kMaxSizeMessageQueue)) {

}

bool SDLMainThreadManager::isInitialized() const {
  if (!m_pImpl) {
    return false;
  }
  if (!m_pQueueImpl) {
    return false;
  }
  return true;
}

void SDLMainThreadManager::processEvents() {
  if (!isInitialized()) {
    return;
  }
  bool doProcessEvents = true;
  while(doProcessEvents) {
    MessageSharedPtr pMessage(m_pQueueImpl->pop_back());
#ifdef DEBUG
    std::cout << "SDLMainThreadManager::processEvents received message" << std::endl;
#endif
    if (pMessage) {
#ifdef DEBUG
      std::cout << "SDLMainThreadManager::processEvents processing message";
      std::cout << std::endl;
#endif
      pMessage->operator()();
    }
    bool doQuit = pollForSDLQuitEvent();
    if (doQuit) {
      doProcessEvents = false;
    }
  }
}

bool SDLMainThreadManager::pollForSDLQuitEvent() {
#ifdef DEBUG
    std::cout << "SDLMainThreadManager::pollForSDLQuitEvent" << std::endl;
#endif
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
#ifdef DEBUG
    std::cout << "Polled event" << std::endl;
#endif
    if (event.type == SDL_QUIT) {
      return true;
    }
  }
  return false;
}

SDLVideoManager SDLMainThreadManager::videoManager() {
  MessageQueueType messageQueue(m_pQueueImpl);
  SDLVideoManager videoManager(m_pImpl, messageQueue);
  return videoManager;
}

bool SDLMainThreadManager::setVideoMode(const RGBVideoFormat& videoFormat) {
  if (!isInitialized()) {
    return false;
  }
  return m_pImpl->setVideoMode(videoFormat);
}

} // VideoCapture
