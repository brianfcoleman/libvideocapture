#include "RGBVideoFrameFactory.hpp"
#include "RGBVideoFrame.hpp"
#include "RGBVideoFormat.hpp"

namespace VideoCapture {

RGBVideoFrameFactory::RGBVideoFrameFactory() {

}

RGBVideoFrameFactory::SampleDataSharedPtr RGBVideoFrameFactory::createSampleData(
    const RGBVideoFormat& videoFormat) {
  if (!videoFormat) {
    SampleDataSharedPtr pSampleData;
    return pSampleData;
  }
  SampleDataSharedPtr pSampleData(new RGBVideoFrame(videoFormat));
  return pSampleData;
}

} // VideoCapture
