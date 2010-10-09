#ifndef VIDEO_CAPTURE_SDL_SAMPLE_CONSUMER_CALLBACK_H
#define VIDEO_CAPTURE_SDL_SAMPLE_CONSUMER_CALLBACK_H

#include "RGBVideoFrame.hpp"
#include "Sample.hpp"
#include "SDLVideoManager.hpp"

namespace VideoCapture {

class SDLSampleConsumerCallback {
 public:
  typedef Sample<RGBVideoFrame> SampleType;
  typedef SampleType& SampleRef;

  SDLSampleConsumerCallback(const SDLVideoManager& sdlVideoManager)
      : m_sdlVideoManager(sdlVideoManager) {

  }

  void operator()(SampleRef sample) {
#ifdef DEBUG
    std::cout << "SDLSampleConsumerCallback" << std::endl;
#endif
    m_sdlVideoManager.renderToScreen(sample);
  }

 private:
  SDLVideoManager m_sdlVideoManager;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SDL_SAMPLE_CONSUMER_CALLBACK_H
