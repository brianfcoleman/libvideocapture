#include <cstdlib>
#include "VideoCaptureDeviceManagerFactory.hpp"
#include "SampleStreamBuilder.hpp"
#include "RGBVideoFrame.hpp"
#include "SDLSampleConsumerCallback.hpp"
#include "SDLVideoManagerFactory.hpp"

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
  typedef RGBVideoSampleStreamBuilder::SampleConsumerType SampleConsumerType;
  typedef RGBVideoSampleStreamBuilder::SampleStreamType RGBVideoSampleStream;

#ifdef DEBUG
  std::cout << "Debug" << std::endl;
#endif

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
  RGBVideoFormat videoFormat(videoFormatList.front());
  bool isInitialized = videoFormat.isInitialized();
  std::cout << "video format " << (isInitialized ? "is " : "is not ");
  std::cout << "initialized" << std::endl;

  SDLVideoManagerFactory sdlVideoManangerFactory;
  SDLVideoManager sdlVideoManager(sdlVideoManangerFactory());

  std::cout << "sdl video manager " << (sdlVideoManager ? "" : "not");
  std::cout << " is initialized" << std::endl;

  sdlVideoManager.startEventProcessor();
  RGBVideoFormat currentVideoFormat(videoCaptureDevice.currentVideoFormat());
  sdlVideoManager.setVideoMode(currentVideoFormat);

  RGBVideoSampleStreamBuilder sampleStreamBuilder(
      RGBVideoSampleStreamBuilder::s_kDefaultMaxCountAllocatedSamples);
  SampleSourceType sampleSource(
      sampleStreamBuilder.connectSampleSource(videoCaptureDevice));
  SDLSampleConsumerCallback sampleConsumerCallback(sdlVideoManager);
  SampleConsumerType sampleConsumer(
      sampleStreamBuilder.connectSampleSinkToPreviousSampleStage(
          sampleSource,
          sampleConsumerCallback));
  bool isReadyToBuild = sampleStreamBuilder.isReadyToBuild();
  std::cout << (isReadyToBuild ? "isReadyToBuild" : "not isReadyToBuild");
  std::cout << std::endl;
  RGBVideoSampleStream sampleStream(sampleStreamBuilder.build());

  bool didStart = videoCaptureDevice.startCapturing();
  std::cout << (didStart ? "Did start" : "Did not start");
  std::cout << " capturing" << std::endl;

  sdlVideoManager.waitForQuitEvent();

  return EXIT_SUCCESS;
}
