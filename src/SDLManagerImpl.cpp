#include "SDLManagerImpl.hpp"
#ifdef DEBUG
#include <iostream>
#endif

namespace VideoCapture {

SDLManagerImpl::SDLManagerImpl(Uint32 flags)
    : m_isInitialized(false) {
  m_isInitialized = initialize(flags);
}

SDLManagerImpl::~SDLManagerImpl() {
  if (!isInitialized()) {
    return;
  }
  SDL_Quit();
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
#ifdef DEBUG
  std::cout << "SDLManagerImpl::renderToScreen" << std::endl;
#endif
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

} // VideoCapture
