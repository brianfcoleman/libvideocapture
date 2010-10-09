#include <cstdlib>
#include <cmath>
#include "VideoCaptureDeviceManagerFactory.hpp"
#include "SampleStreamBuilder.hpp"
#include "RGBVideoFrame.hpp"
#include "SDLSampleConsumerCallback.hpp"
#include "SDLMainThreadManager.hpp"
#include "RotateBy180DegreesSampleConverter.hpp"

int main(int argc, char *argv[]) {
  using namespace VideoCapture;
  typedef VideoCaptureDeviceManager::VideoCaptureDeviceList
      VideoCaptureDeviceList;
  typedef VideoCaptureDeviceList::size_type size_type;
  typedef VideoCaptureDevice::RGBVideoFormatList RGBVideoFormatList;
  typedef RGBVideoFormatList::iterator RGBVideoFormatListIterator;
  typedef Sample<RGBVideoFrame> RGBVideoSample;
  typedef SampleStreamBuilder<
    RGBVideoSample,
    RGBVideoSample> RGBVideoSampleStreamBuilder;
  typedef RGBVideoSampleStreamBuilder::SampleSourceType SampleSourceType;
  typedef RGBVideoSampleStreamBuilder::SampleConsumerType SampleConsumerType;
  typedef RGBVideoSampleStreamBuilder::SampleStreamType RGBVideoSampleStream;
  typedef SampleConverter<
    RGBVideoSample,
    RGBVideoSample> SampleConverterType;
  typedef SampleConverterType::SampleConverterSharedPtr SampleConverterSharedPtr;
  typedef SampleProcessor<
    RGBVideoSample,
    RGBVideoSample> SampleProcessorType;

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
  RGBVideoFormat videoFormat;
  for (RGBVideoFormatListIterator iterator(videoFormatList.begin());
       iterator != videoFormatList.end();
       ++iterator) {
    RGBVideoFormat currentVideoFormat(*iterator);
    if (!currentVideoFormat) {
      continue;
    }
    IntegerSize sizePixels(currentVideoFormat.sizePixels());
    if (sizePixels.width() != 640) {
      continue;
    }
    if (sizePixels.height() != 480) {
      continue;
    }
    videoFormat = currentVideoFormat;
  }
  bool isInitialized = videoFormat.isInitialized();
  std::cout << "video format " << (isInitialized ? "is " : "is not ");
  std::cout << "initialized" << std::endl;
  videoCaptureDevice.setCurrentVideoFormat(videoFormat);
  RGBVideoFormat currentVideoFormat(videoCaptureDevice.currentVideoFormat());
  boost::int32_t angleRotationDegrees =
      currentVideoFormat.angleRotationDegrees();
  bool isUpsideDown = (abs(angleRotationDegrees) == kAngleHalfRotationDegrees);

  SDLMainThreadManager sdlMainThreadManager;
  SDLVideoManager sdlVideoManager(sdlMainThreadManager.videoManager());

  std::cout << "sdl video manager " << (sdlVideoManager ? "" : "not");
  std::cout << " is initialized" << std::endl;

  RGBVideoSampleStreamBuilder sampleStreamBuilder(
      RGBVideoSampleStreamBuilder::s_kDefaultMaxCountAllocatedSamples);
  SampleSourceType sampleSource(
      sampleStreamBuilder.connectSampleSource(videoCaptureDevice));
  SampleProcessorType previousSampleStage;
  if (isUpsideDown) {
    SampleConverterSharedPtr pRotateBy180DegreesSampleConverter(
        sampleConverterSharedPtr<
        RGBVideoSample,
        RGBVideoSample,
        RotateBy180DegreesSampleConverter>());
    previousSampleStage =
        sampleStreamBuilder.connectSampleConverterToPreviousSampleStage<
          SampleSourceType,
          RGBVideoSample>(
            sampleSource,
            pRotateBy180DegreesSampleConverter);
  }
  SDLSampleConsumerCallback sampleConsumerCallback(sdlVideoManager);
  SampleConsumerType sampleConsumer;
  if (isUpsideDown) {
      sampleConsumer =
          sampleStreamBuilder.connectSampleSinkToPreviousSampleStage(
              previousSampleStage,
              sampleConsumerCallback);
  } else {
    sampleConsumer =
        sampleStreamBuilder.connectSampleSinkToPreviousSampleStage(
            sampleSource,
            sampleConsumerCallback);
  }
  bool isReadyToBuild = sampleStreamBuilder.isReadyToBuild();
  std::cout << (isReadyToBuild ? "isReadyToBuild" : "not isReadyToBuild");
  std::cout << std::endl;
  RGBVideoSampleStream sampleStream(sampleStreamBuilder.build());

  bool didStart = videoCaptureDevice.startCapturing();
  std::cout << (didStart ? "Did start" : "Did not start");
  std::cout << " capturing" << std::endl;

  sdlMainThreadManager.setVideoMode(currentVideoFormat);
  sdlMainThreadManager.processEvents();

  return EXIT_SUCCESS;
}
