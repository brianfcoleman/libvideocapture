#include "SDLSurfaceLock.hpp"

namespace VideoCapture {

SDLSurfaceLock::SDLSurfaceLock(SDL_Surface* pSurface)
    : m_isSurfaceLocked(false),
      m_pSurface(pSurface) {
  m_isSurfaceLocked = lockSurface();
}

SDLSurfaceLock::~SDLSurfaceLock() {
  unlockSurface();
}

bool SDLSurfaceLock::isSurfaceLocked() const {
  return m_isSurfaceLocked;
}

bool SDLSurfaceLock::mustLockSurface() const {
  return SDL_MUSTLOCK(m_pSurface);
}

bool SDLSurfaceLock::lockSurface() {
  if (!mustLockSurface()) {
    return true;
  }
  int result = SDL_LockSurface(m_pSurface);
  if (result < 0) {
    return false;
  }
  return true;
}

void SDLSurfaceLock::unlockSurface() {
  if (!mustLockSurface()) {
    return;
  }
  if (!isSurfaceLocked()) {
    return;
  }
  SDL_UnlockSurface(m_pSurface);
}

}
