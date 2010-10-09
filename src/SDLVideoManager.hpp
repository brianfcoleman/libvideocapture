#ifndef VIDEO_CAPTURE_SDL_VIDEO_MANAGER_H
#define VIDEO_CAPTURE_SDL_VIDEO_MANAGER_H

#include "boost/weak_ptr.hpp"
#include "boost/shared_ptr.hpp"
#include "MessageSenderFactory.hpp"
#include "MessageReceiver.hpp"
#include "Message.hpp"
#include "MessageQueue.hpp"
#include "RGBVideoFormat.hpp"
#include "RGBVideoFrame.hpp"
#include "Sample.hpp"

namespace VideoCapture {

class SDLVideoManagerImpl;

class SDLVideoManager {
 public:
  typedef MessageSenderFactory<SDLVideoManagerImpl> MessageSenderFactoryType;
  typedef MessageReceiver<MessageSenderFactoryType> MessageReceiverType;
  typedef boost::shared_ptr<MessageReceiverType> MessageReceiverSharedPtr;
  typedef Message MessageType;
  typedef MessageQueue<MessageType> MessageQueueType;
  typedef SDLVideoManagerImpl ImplType;
  typedef boost::shared_ptr<ImplType> ImplPtr;
  typedef boost::weak_ptr<ImplType> ImplWeakPtr;
  typedef Sample<RGBVideoFrame> RGBVideoSample;

  SDLVideoManager();
  SDLVideoManager(
      const ImplPtr& pImpl,
      const MessageQueueType& messageQueue,
      const MessageReceiverSharedPtr& pMessageReceiver);
  bool setVideoMode(const RGBVideoFormat& videoFormat);
  bool renderToScreen(RGBVideoSample& videoSample);
  // TODO Possibly change the api to this member function
  // to match that of the impl
  bool startEventProcessor();
  bool waitForQuitEvent();
  bool isInitialized() const;
  operator bool() const {
    return isInitialized();
  }
 private:
  ImplWeakPtr m_pWeakImpl;
  mutable MessageQueueType m_messageQueue;
  MessageReceiverSharedPtr m_pMessageReceiver;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SDL_VIDEO_MANAGER_H
