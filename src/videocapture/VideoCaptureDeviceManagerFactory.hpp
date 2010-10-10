#ifndef VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_MANAGER_FACTORY_H
#define VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_MANAGER_FACTORY_H

#include "VideoCaptureDeviceManager.hpp"

namespace VideoCapture {

class VideoCaptureDeviceManagerFactory {
 public:
  VideoCaptureDeviceManagerFactory();
  VideoCaptureDeviceManager createVideoCaptureDeviceManager();
  VideoCaptureDeviceManager operator()() {
    return createVideoCaptureDeviceManager();
  }
 private:
  static const std::size_t s_kMaxSizeMessageQueue = 8;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_MANAGER_FACTORY_H
