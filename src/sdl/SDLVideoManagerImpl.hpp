#ifndef VIDEO_CAPTURE_SDL_VIDEO_MANAGER_IMPL_H
#define VIDEO_CAPTURE_SDL_VIDEO_MANAGER_IMPL_H

#include "SDLManagerImpl.hpp"

namespace VideoCapture {

class SDLVideoManagerImpl : public SDLManagerImpl {
 public:
  typedef boost::shared_ptr<SDLVideoManagerImpl> SDLVideoManagerImplSharedPtr;
  SDLVideoManagerImpl()
      : SDLManagerImpl(SDL_INIT_VIDEO) {

  }
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SDL_VIDEO_MANAGER_IMPL_H
