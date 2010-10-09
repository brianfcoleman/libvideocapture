#ifndef VIDEO_CAPTURE_SDL_VIDEO_MANAGER_FACTORY_H
#define VIDEO_CAPTURE_SDL_VIDEO_MANAGER_FACTORY_H

#include "SDLVideoManager.hpp"

namespace VideoCapture {

class SDLVideoManagerFactory {
 public:
  SDLVideoManagerFactory();
  SDLVideoManager createSDLVideoManager();
  SDLVideoManager operator()() {
    return createSDLVideoManager();
  }
 private:
  static const std::size_t s_kMaxSizeMessageQueue = 8;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SDL_VIDEO_MANAGER_FACTORY_H
