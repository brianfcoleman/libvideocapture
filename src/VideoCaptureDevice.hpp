#ifndef VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_H
#define VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_H

#define BOOST_BIND_ENABLE_STDCALL
#define BOOST_MEM_FN_ENABLE_STDCALL

#include <string>
#include <list>
#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"
#include "boost/function.hpp"
#include "RGBVideoFormat.hpp"
#include "ByteBuffer.hpp"
#include "Message.hpp"
#include "MessageQueue.hpp"

namespace VideoCapture {

class VideoCaptureDeviceImpl;

class VideoCaptureDevice {
 public:
  typedef boost::shared_ptr<VideoCaptureDeviceImpl> ImplPtr;
  typedef boost::weak_ptr<VideoCaptureDeviceImpl> ImplWeakPtr;
  typedef boost::function<void (ByteBuffer)> SampleProducerCallbackType;
  typedef std::list<SampleProducerCallbackType> SampleProducerCallbackList;
  typedef std::list<RGBVideoFormat> RGBVideoFormatList;
  typedef Message MessageType;
  typedef MessageQueue<MessageType> MessageQueueType;

  VideoCaptureDevice();
  explicit VideoCaptureDevice(
      const ImplPtr& pImpl,
      const MessageQueueType& messageQueue);
  bool isInitialized() const;
  std::string name() const;
  bool startCapturing(
      const SampleProducerCallbackList& sampleProducerCallbackList);
  bool stopCapturing();
  double countFramesCapturedPerSecond() const;
  RGBVideoFormatList videoFormatList() const;
  RGBVideoFormat currentVideoFormat() const;
  bool setCurrentVideoFormat(const RGBVideoFormat& videoFormat);
  operator bool() const {
    return isInitialized();
  }
 private:
  static const std::string s_kEmptyString;
  ImplPtr lockImplPtr() const;
  ImplWeakPtr m_pImpl;
  mutable MessageQueueType m_messageQueue;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_H
