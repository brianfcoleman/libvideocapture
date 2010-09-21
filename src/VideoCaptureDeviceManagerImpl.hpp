#ifndef VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_MANAGER_IMPL_H
#define VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_MANAGER_IMPL_H

#include <list>
#include "boost/utility.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "DirectShow.hpp"
#include "COMInitializer.hpp"
#include "Message.hpp"
#include "MessageQueue.hpp"
#include "VideoCaptureDeviceImpl.hpp"
#include "VideoCaptureDevice.hpp"

namespace VideoCapture {

class VideoCaptureDeviceManagerImpl : private boost::noncopyable {
 public:
  typedef boost::shared_ptr<
   VideoCaptureDeviceImpl> VideoCaptureDeviceImplSharedPtr;
  typedef std::list<VideoCaptureDeviceImplSharedPtr> VideoCaptureDeviceImplList;
  typedef VideoCaptureDeviceImplList::size_type SizeType;
  typedef VideoCaptureDeviceImplList::const_iterator
  VideoCaptureDeviceImplConstIterator;
  typedef Message MessageType;
  typedef MessageQueue<MessageType> MessageQueueType;
  typedef std::list<VideoCaptureDevice> VideoCaptureDeviceList;
  typedef boost::uuids::uuid Uuid;
  typedef boost::uuids::random_generator UuidGenerator;
  typedef boost::shared_ptr<ICreateDevEnum> ICreateDevEnumSharedPtr;
  typedef boost::shared_ptr<IEnumMoniker> IEnumMonikerSharedPtr;
  typedef boost::shared_ptr<IMoniker> IMonikerSharedPtr;

  explicit VideoCaptureDeviceManagerImpl(const MessageQueueType& messageQueue);
  VideoCaptureDeviceList videoCaptureDeviceList() const;
  MessageQueueType messageQueue();
  bool isInitialized() const;
  operator bool() const {
    return isInitialized();
  }
 private:
  bool initialize();
  bool initVideoCaptureDeviceImplList();
  bool m_isInitialized;
  COMInitializer m_comInitializer;
  MessageQueueType m_messageQueue;
  VideoCaptureDeviceImplList m_videoCaptureDeviceImplList;
  UuidGenerator m_uuidGenerator;
  QeditTypeLibrary m_qeditTypeLibrary;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_MANAGER_IMPL_H
