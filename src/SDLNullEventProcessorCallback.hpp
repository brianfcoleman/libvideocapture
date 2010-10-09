#ifndef VIDEO_CAPTURE_SDL_NULL_EVENT_PROCESSOR_CALLBACK
#define VIDEO_CAPTURE_SDL_NULL_EVENT_PROCESSOR_CALLBACK

#include "SDL.h"

namespace VideoCapture {

class SDLNullEventProcessorCallback {
 public:
  SDLNullEventProcessorCallback() {

  }

  bool operator()(const SDL_Event& event) {
    return false;
  }
};

}

#endif // VIDEO_CAPTURE_SDL_NULL_EVENT_PROCESSOR_CALLBACK
