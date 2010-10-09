#ifndef VIDEO_CAPTURE_SDL_SURFACE_LOCK_H
#define VIDEO_CAPTURE_SDL_SURFACE_LOCK_H

#include "boost/utility.hpp"
#include "SDL.h"

namespace VideoCapture {

class SDLSurfaceLock : private boost::noncopyable {
 public:
  SDLSurfaceLock(SDL_Surface* pSurface);
  ~SDLSurfaceLock();
  bool isSurfaceLocked() const;
  operator bool() const {
    return isSurfaceLocked();
  }
 private:
  bool mustLockSurface() const;
  bool lockSurface();
  void unlockSurface();
  bool m_isSurfaceLocked;
  SDL_Surface* m_pSurface;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SDL_SURFACE_LOCK_H
