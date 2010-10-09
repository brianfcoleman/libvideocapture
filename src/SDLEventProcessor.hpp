#ifndef VIDEO_CAPTURE_SDL_EVENT_PROCESSOR_H
#define VIDEO_CAPTURE_SDL_EVENT_PROCESSOR_H

#include "boost/utility.hpp"
#include "boost/thread.hpp"
#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "SDL.h"

namespace VideoCapture {

class SDLEventProcessor : boost::noncopyable {
 public:
  typedef boost::shared_ptr<SDLEventProcessor> SDLEventProcessorSharedPtr;
  typedef boost::function<bool (const SDL_Event&)> EventProcessorCallback;
  SDLEventProcessor(const EventProcessorCallback& eventProcessorCallback);
  ~SDLEventProcessor();
  void joinThread();
 private:
  void interruptThread();
  boost::thread m_thread;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SDL_EVENT_PROCESSOR_H
