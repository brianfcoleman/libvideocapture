#ifndef VIDEO_CAPTURE_SDL_MANAGER_IMPL_H
#define VIDEO_CAPTURE_SDL_MANAGER_IMPL_H

#include "boost/utility.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/function.hpp"
#include "SDL.h"
#include "RGBVideoFormat.hpp"
#include "RGBVideoFrame.hpp"
#include "Sample.hpp"
#include "SDLSurface.hpp"
#include "SDLRenderer.hpp"

namespace VideoCapture {

class SDLManagerImpl : private boost::noncopyable {
 public:
  typedef boost::shared_ptr<SDLManagerImpl> SDLManagerImplSharedPtr;
  typedef SDLSurface::SDLSurfaceSharedPtr SDLSurfaceSharedPtr;
  typedef std::size_t SizeType;
  typedef Sample<RGBVideoFrame> RGBVideoSample;
  typedef SDLRenderer::SDLRendererSharedPtr SDLRendererSharedPtr;

  SDLManagerImpl(Uint32 flags);
  virtual ~SDLManagerImpl();
  bool setVideoMode(const RGBVideoFormat& videoFormat);
  bool renderToScreen(RGBVideoSample& videoSample);
  bool isInitialized() const;
  operator bool() {
    return isInitialized();
  }
 private:
  bool initialize(Uint32 flags);
  bool m_isInitialized;
  SDLRendererSharedPtr m_pRenderer;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SDL_MANAGER_IMPL_H
