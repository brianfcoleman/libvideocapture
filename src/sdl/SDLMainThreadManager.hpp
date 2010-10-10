#ifndef VIDEO_CAPTURE_SDL_MAIN_THREAD_MANAGER_H
#define VIDEO_CAPTURE_SDL_MAIN_THREAD_MANAGER_H

#include "boost/utility.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"
#include "SDL.h"
#include "SDLVideoManager.hpp"
#include "RGBVideoFormat.hpp"
#include "Message.hpp"
#include "MessageQueue.hpp"

namespace VideoCapture {

class SDLVideoManagerImpl;

class SDLMainThreadManager : private boost::noncopyable {
 public:
  typedef boost::shared_ptr<SDLVideoManagerImpl> ImplPtr;
  typedef boost::weak_ptr<SDLVideoManagerImpl> ImplWeakPtr;
  typedef Message MessageType;
  typedef MessageQueue<MessageType> MessageQueueType;
  typedef MessageQueueType::QueueImplType MessageQueueImplType;
  typedef MessageQueueType::ImplPtr MessageQueueImplPtr;
  typedef MessageQueueType::SizeType SizeType;
  typedef MessageQueueType::MessageSharedPtr MessageSharedPtr;

  SDLMainThreadManager();
  SDLVideoManager videoManager();
  bool setVideoMode(const RGBVideoFormat& videoFormat);
  void processEvents();
  bool isInitialized() const;
  operator bool() const {
    return isInitialized();
  }
 private:
  static const SizeType s_kMaxSizeMessageQueue = 8;
  bool pollForSDLQuitEvent();
  ImplPtr m_pImpl;
  MessageQueueImplPtr m_pQueueImpl;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SDL_MAIN_THREAD_MANAGER_H
