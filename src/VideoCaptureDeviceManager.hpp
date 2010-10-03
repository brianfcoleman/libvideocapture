#ifndef VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_MANAGER_H
#define VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_MANAGER_H

#include <list>
#include "boost/weak_ptr.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/function.hpp"
#include "MessageSenderFactory.hpp"
#include "MessageReceiver.hpp"
#include "MessageQueue.hpp"
#include "Message.hpp"
#include "VideoCaptureDevice.hpp"

namespace VideoCapture {

class VideoCaptureDeviceManagerImpl;

class VideoCaptureDeviceManager {
 public:
  typedef boost::weak_ptr<VideoCaptureDeviceManagerImpl> ImplWeakPtr;
  typedef boost::shared_ptr<VideoCaptureDeviceManagerImpl> ImplPtr;
  typedef Message MessageType;
  typedef MessageQueue<MessageType> MessageQueueType;
  typedef MessageSenderFactory<
    VideoCaptureDeviceManagerImpl> MessageSenderFactoryType;
  typedef MessageReceiver<MessageSenderFactoryType> MessageReceiverType;
  typedef boost::shared_ptr<MessageReceiverType> MessageReceiverSharedPtr;
  typedef std::list<VideoCaptureDevice> VideoCaptureDeviceList;

  VideoCaptureDeviceManager();
  VideoCaptureDeviceManager(
      const ImplPtr& pImpl,
      const MessageQueueType& messageQueue,
      const MessageReceiverSharedPtr& pMessageReceiver);
  VideoCaptureDeviceList videoCaptureDeviceList() const;
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

#endif // VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_MANAGER_H
