#ifndef VIDEO_CAPTURE_SDL_RENDERER_H
#define VIDEO_CAPTURE_SDL_RENDERER_H

#include "boost/utility.hpp"
#include "boost/shared_ptr.hpp"
#include "SDL.h"
#include "RGBVideoFormat.hpp"
#include "SDLSurface.hpp"

namespace VideoCapture {

class SDLRenderer : boost::noncopyable {
 public:
  typedef std::size_t SizeType;
  typedef SDLSurface::SDLSurfaceSharedPtr SDLSurfaceSharedPtr;
  typedef SDLSurface::RGBVideoSample RGBVideoSample;
  typedef boost::shared_ptr<SDLRenderer> SDLRendererSharedPtr;

  SDLRenderer(const RGBVideoFormat& videoFormat);
  bool renderToScreen(RGBVideoSample& videoSample);
  bool isInitialized() const;
  operator bool() const {
    return isInitialized();
  }
 private:
  bool blitBackBufferToScreen();
  SDL_Rect convertSizeToRect(const IntegerSize& size);
  bool initialize(const RGBVideoFormat& videoFormat);
  bool m_isInitialized;
  SDLSurfaceSharedPtr m_pScreenSurface;
  SDLSurfaceSharedPtr m_pBackBufferSurface;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SDL_RENDERER_H
