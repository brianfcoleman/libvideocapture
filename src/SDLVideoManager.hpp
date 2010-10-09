#ifndef VIDEO_CAPTURE_SDL_VIDEO_MANAGER_H
#define VIDEO_CAPTURE_SDL_VIDEO_MANAGER_H

#include "boost/weak_ptr.hpp"
#include "boost/shared_ptr.hpp"
#include "Message.hpp"
#include "MessageQueue.hpp"
#include "RGBVideoFormat.hpp"
#include "RGBVideoFrame.hpp"
#include "Sample.hpp"

namespace VideoCapture {

class SDLVideoManagerImpl;

class SDLVideoManager {
 public:
  typedef Message MessageType;
  typedef MessageQueue<MessageType> MessageQueueType;
  typedef MessageQueueType::MessageSharedPtr MessageSharedPtr;
  typedef SDLVideoManagerImpl ImplType;
  typedef boost::shared_ptr<ImplType> ImplPtr;
  typedef boost::weak_ptr<ImplType> ImplWeakPtr;
  typedef Sample<RGBVideoFrame> RGBVideoSample;

  SDLVideoManager();
  SDLVideoManager(
      const ImplPtr& pImpl,
      const MessageQueueType& messageQueue);
  bool setVideoMode(const RGBVideoFormat& videoFormat);
  bool renderToScreen(RGBVideoSample& videoSample);
  bool isInitialized() const;
  operator bool() const {
    return isInitialized();
  }
 private:
  ImplWeakPtr m_pWeakImpl;
  mutable MessageQueueType m_messageQueue;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SDL_VIDEO_MANAGER_H
