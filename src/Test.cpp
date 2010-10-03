#include <cstdlib>
#include "VideoCaptureDeviceManagerFactory.hpp"
#include "SampleStreamBuilder.hpp"
#include "RGBVideoFrame.hpp"

int main() {
  using namespace VideoCapture;
  typedef VideoCaptureDeviceManager::VideoCaptureDeviceList
      VideoCaptureDeviceList;
  typedef VideoCaptureDeviceList::size_type size_type;
  typedef VideoCaptureDevice::RGBVideoFormatList RGBVideoFormatList;
  typedef Sample<RGBVideoFrame> RGBVideoSample;
  typedef SampleStreamBuilder<
    RGBVideoSample,
    RGBVideoSample> RGBVideoSampleStreamBuilder;
  typedef RGBVideoSampleStreamBuilder::SampleSourceType SampleSourceType;
  typedef RGBVideoSampleStreamBuilder::SampleSinkType SampleSinkType;
  typedef RGBVideoSampleStreamBuilder::SampleStreamType RGBVideoSampleStream;

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

  RGBVideoSampleStreamBuilder sampleStreamBuilder(
      RGBVideoSampleStreamBuilder::s_kDefaultMaxCountAllocatedSamples);
  SampleSourceType sampleSource(
      sampleStreamBuilder.connectSampleSource(videoCaptureDevice));
  SampleSinkType sampleSink(
      sampleStreamBuilder.connectSampleSink(sampleSource));
  bool isReadyToBuild = sampleStreamBuilder.isReadyToBuild();
  std::cout << (isReadyToBuild ? "isReadyToBuild" : "not isReadyToBuild");
  std::cout << std::endl;
  RGBVideoSampleStream sampleStream(sampleStreamBuilder.build());

  bool didStart = videoCaptureDevice.startCapturing();
  std::cout << (didStart ? "Did start" : "Did not start");
  std::cout << " capturing" << std::endl;

  return EXIT_SUCCESS;
}
