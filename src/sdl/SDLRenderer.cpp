#include "SDLRenderer.hpp"
#ifdef DEBUG
#include <iostream>
#endif

namespace VideoCapture {

SDLRenderer::SDLRenderer(const RGBVideoFormat& videoFormat)
    : m_isInitialized(false) {
  m_isInitialized = initialize(videoFormat);
}

bool SDLRenderer::renderToScreen(RGBVideoSample& videoSample) {
#ifdef DEBUG
  std::cout << "SDLRenderer::renderToScreen" << std::endl;
#endif
  if (!isInitialized()) {
    return false;
  }
  if (!m_pBackBufferSurface->renderToSurface(videoSample)) {
    return false;
  }
  if (!blitBackBufferToScreen()) {
    return false;
  }
  return true;
}

bool SDLRenderer::blitBackBufferToScreen() {
  IntegerSize sizeBackBuffer(m_pBackBufferSurface->sizePixels());
  SDL_Rect rectBackBuffer(convertSizeToRect(sizeBackBuffer));
  IntegerSize sizeScreen(m_pScreenSurface->sizePixels());
  SDL_Rect rectScreen(convertSizeToRect(sizeScreen));
  int result = SDL_BlitSurface(
      m_pBackBufferSurface->rawSurfacePtr(),
      &rectBackBuffer,
      m_pScreenSurface->rawSurfacePtr(),
      &rectScreen);
  bool didRender = (result == 0);
  if (!didRender) {
    return false;
  }
  SDL_UpdateRect(
      m_pScreenSurface->rawSurfacePtr(),
      rectScreen.x,
      rectScreen.y,
      rectScreen.w,
      rectScreen.h);
#ifdef DEBUG
  std::cout << "SDLRenderer::blitBackBufferToScreen result " << result << " ";
  std::cout << (didRender ? "" : "not ") << "did render" << std::endl;
  std::cout << "sizeBackBuffer " << sizeBackBuffer << std::endl;
  std::cout << "sizeScreen " << sizeScreen << std::endl;
#endif
  return didRender;
}

SDL_Rect SDLRenderer::convertSizeToRect(const IntegerSize& size) {
  SDL_Rect rect;
  rect.x = 0;
  rect.y = 0;
  rect.w = 0;
  rect.h = 0;
  SizeType width = size.width();
  if (!width) {
    return rect;
  }
  SizeType height = size.height();
  if (!height) {
    return rect;
  }
  rect.w = width;
  rect.h = height;
  return rect;
}

bool SDLRenderer::isInitialized() const {
  return m_isInitialized;
}

bool SDLRenderer::initialize(const RGBVideoFormat& videoFormat) {
  if (!videoFormat) {
    return false;
  }
  IntegerSize sizePixels(videoFormat.sizePixels());
  SizeType width = sizePixels.width();
  if (!width) {
    return false;
  }
  SizeType height = sizePixels.height();
  if (!height) {
    return false;
  }
  SizeType bitsPerPixel = videoFormat.bitsPerPixel();
  if (!bitsPerPixel) {
    return false;
  }
  Uint32 flags = SDL_SWSURFACE;
  int bitsPerPixelScreen = SDL_VideoModeOK(width, height, bitsPerPixel, flags);
  if (bitsPerPixelScreen != bitsPerPixel) {
    return false;
  }
  SDLSurfaceSharedPtr pScreenSurface(
      new SDLSurface(SDL_SetVideoMode(width, height, bitsPerPixel, flags)));
  if (!pScreenSurface) {
    return false;
  }
  m_pScreenSurface = pScreenSurface;
  SDLSurfaceSharedPtr pBackBufferSurface(
      SDLSurface::createRGBSurfaceSharedPtr(videoFormat));
  if (!pBackBufferSurface) {
    return false;
  }
  m_pBackBufferSurface = pBackBufferSurface;
  return true;
}

} // VideoCapture
