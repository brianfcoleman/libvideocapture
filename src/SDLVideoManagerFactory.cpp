#include "SDLVideoManagerFactory.hpp"
#include "SDLVideoManagerImpl.hpp"
#include "MessageSenderInterfaceFactory.hpp"

namespace VideoCapture {

SDLVideoManagerFactory::SDLVideoManagerFactory() {

}

SDLVideoManager SDLVideoManagerFactory::createSDLVideoManager() {
  typedef MessageSenderInterfaceFactory<SDLVideoManager> FactoryType;
  FactoryType factory;
  SDLVideoManager sdlVideoManager(factory(s_kMaxSizeMessageQueue));
  return sdlVideoManager;
}

} // VideoCapture
