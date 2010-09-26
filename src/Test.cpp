#include <cstdlib>
#include "VideoCaptureDeviceManagerFactory.hpp"

int main() {
  using namespace VideoCapture;
  typedef VideoCaptureDeviceManager::VideoCaptureDeviceList
      VideoCaptureDeviceList;
  typedef VideoCaptureDeviceList::size_type size_type;
  typedef VideoCaptureDevice::RGBVideoFormatList RGBVideoFormatList;
  typedef VideoCaptureDevice::SampleProducerCallbackList
      SampleProducerCallbackList;
  VideoCaptureDeviceManagerFactory videoCaptureDeviceManagerFactory;
  VideoCaptureDeviceManager videoCaptureDeviceManager(
      videoCaptureDeviceManagerFactory());
  VideoCaptureDeviceList videoCaptureDeviceList(
      videoCaptureDeviceManager.videoCaptureDeviceList());
  size_type countVideoCaptureDevices = videoCaptureDeviceList.size();
  std::cout << "Found " << countVideoCaptureDevices;
  std::cout << " video capture devices" << std::endl;
  VideoCaptureDevice videoCaptureDevice(videoCaptureDeviceList.front());
  std::string name(videoCaptureDevice.name());
  std::cout << "Name : " << name << std::endl;
  RGBVideoFormatList videoFormatList(videoCaptureDevice.videoFormatList());
  size_type countVideoFormats = videoFormatList.size();
  std::cout << "Found " << countVideoFormats;
  std::cout << " video formats" << std::endl;
  SampleProducerCallbackList emptyList;
  bool didStart = videoCaptureDevice.startCapturing(emptyList);
  std::cout << (didStart ? "Did start" : "Did not start");
  std::cout << " capturing" << std::endl;
  return EXIT_SUCCESS;
}
