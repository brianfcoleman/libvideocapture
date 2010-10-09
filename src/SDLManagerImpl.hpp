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
#include "SDLEventProcessor.hpp"
#include "SDLNullEventProcessorCallback.hpp"
#include "Message.hpp"
#include "MessageQueue.hpp"

namespace VideoCapture {

class SDLManagerImpl : private boost::noncopyable {
 public:
  typedef boost::shared_ptr<SDLManagerImpl> SDLManagerImplSharedPtr;
  typedef SDLSurface::SDLSurfaceSharedPtr SDLSurfaceSharedPtr;
  typedef std::size_t SizeType;
  typedef Sample<RGBVideoFrame> RGBVideoSample;
  typedef SDLEventProcessor::EventProcessorCallback EventProcessorCallback;
  typedef SDLEventProcessor::SDLEventProcessorSharedPtr
  SDLEventProcessorSharedPtr;
  typedef SDLRenderer::SDLRendererSharedPtr SDLRendererSharedPtr;
  typedef Message MessageType;
  typedef MessageQueue<MessageType> MessageQueueType;

  SDLManagerImpl(Uint32 flags, const MessageQueueType& messageQueue);
  virtual ~SDLManagerImpl();
  MessageQueueType messageQueue();
  bool setVideoMode(const RGBVideoFormat& videoFormat);
  bool renderToScreen(RGBVideoSample& videoSample);
  bool startEventProcessor(
      const EventProcessorCallback& eventProcessorCallback =
      SDLNullEventProcessorCallback());
  bool waitForQuitEvent();
  bool isInitialized() const;
  operator bool() {
    return isInitialized();
  }
 private:
  bool initialize(Uint32 flags);
  bool m_isInitialized;
  MessageQueueType m_messageQueue;
  SDLRendererSharedPtr m_pRenderer;
  SDLEventProcessorSharedPtr m_pEventProcessor;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SDL_MANAGER_IMPL_H
