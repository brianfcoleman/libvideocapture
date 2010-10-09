#include "SDLEventProcessor.hpp"

namespace VideoCapture {

static void eventProcessorLoop(
    SDLEventProcessor::EventProcessorCallback eventProcessorCallback);

static void eventProcessorLoop(
    SDLEventProcessor::EventProcessorCallback eventProcessorCallback) {
  try {
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
      bool isHandled = eventProcessorCallback(event);
      if (isHandled) {
        continue;
      }
      if (event.type == SDL_QUIT) {
        return;
      }
      if (boost::this_thread::interruption_requested()) {
        return;
      }
    }
  } catch (boost::thread_interrupted exception) {

  }
}

SDLEventProcessor::SDLEventProcessor(
    const EventProcessorCallback& eventProcessorCallback)
    : m_thread(
        boost::bind(&eventProcessorLoop, eventProcessorCallback)) {

}

SDLEventProcessor::~SDLEventProcessor() {
  interruptThread();
}

void SDLEventProcessor::interruptThread() {
  m_thread.interrupt();
}

void SDLEventProcessor::joinThread() {
  try {
    m_thread.join();
  } catch (boost::thread_interrupted exception) {

  }
}

} // VideoCapture
