#include <cstdlib>
#include "DirectShow.hpp"
#include "VideoCaptureDeviceManagerFactory.hpp"

int main() {
  using namespace VideoCapture;
  VideoCaptureDeviceManagerFactory videoCaptureDeviceManagerFactory;
  VideoCaptureDeviceManager videoCaptureDeviceManager(
      videoCaptureDeviceManagerFactory());
  return EXIT_SUCCESS;
}
