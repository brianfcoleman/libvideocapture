#include "VideoCaptureDeviceManagerFactory.hpp"
#include "VideoCaptureDeviceManagerImpl.hpp"
#include "MessageSenderInterfaceFactory.hpp"

namespace VideoCapture {

VideoCaptureDeviceManagerFactory::VideoCaptureDeviceManagerFactory() {

}

VideoCaptureDeviceManager
VideoCaptureDeviceManagerFactory::createVideoCaptureDeviceManager() {
  typedef MessageSenderInterfaceFactory<VideoCaptureDeviceManager>
      FactoryType;
  FactoryType factory;
  VideoCaptureDeviceManager videoCaptureDeviceManager(
      factory(s_kMaxSizeMessageQueue));
  return videoCaptureDeviceManager;
}

} // VideoCapture
