#include <cstdlib>
#include "DirectShow.hpp"
#include "VideoCaptureDeviceManagerFactory.hpp"

int main() {
  using namespace VideoCapture;
  typedef VideoCaptureDeviceManager::VideoCaptureDeviceList
      VideoCaptureDeviceList;
  typedef VideoCaptureDeviceList::size_type size_type;
  VideoCaptureDeviceManagerFactory videoCaptureDeviceManagerFactory;
  VideoCaptureDeviceManager videoCaptureDeviceManager(
      videoCaptureDeviceManagerFactory());
  VideoCaptureDeviceList videoCaptureDeviceList(
      videoCaptureDeviceManager.videoCaptureDeviceList());
  size_type countVideoCaptureDevices = videoCaptureDeviceList.size();
  std::cout << "Found " << countVideoCaptureDevices;
  std::cout << " video capture devices" << std::endl;
  return EXIT_SUCCESS;
}
