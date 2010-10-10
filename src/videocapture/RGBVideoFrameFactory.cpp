#include "RGBVideoFrameFactory.hpp"
#include "RGBVideoFrame.hpp"
#include "RGBVideoFormat.hpp"

namespace VideoCapture {

template<
  typename RGBImageType> static RGBVideoFrameFactory::SampleDataSharedPtr
createRGBImageAndAssignToSampleData(
    const RGBVideoFormat& videoFormat);

RGBVideoFrameFactory::RGBVideoFrameFactory() {

}

RGBVideoFrameFactory::SampleDataSharedPtr RGBVideoFrameFactory::createSampleData(
    const RGBVideoFormat& videoFormat) {
  if (!videoFormat) {
    SampleDataSharedPtr pSampleData;
    return pSampleData;
  }
  RGBFormat rgbFormat = videoFormat.rgbFormat();
  switch (rgbFormat) {
    case RGB888:
      return createRGBImageAndAssignToSampleData<rgb8_image_t>(videoFormat);
    case BGR888:
      return createRGBImageAndAssignToSampleData<bgr8_image_t>(videoFormat);
    case RGBA8888:
      return createRGBImageAndAssignToSampleData<rgba8_image_t>(videoFormat);
    case ABGR8888:
      return createRGBImageAndAssignToSampleData<abgr8_image_t>(videoFormat);
  }
  SampleDataSharedPtr pSampleData;
  return pSampleData;
}

template<
  typename RGBImageType> static RGBVideoFrameFactory::SampleDataSharedPtr
createRGBImageAndAssignToSampleData(
    const RGBVideoFormat& videoFormat) {
  typedef RGBVideoFrameFactory::SampleDataSharedPtr SampleDataSharedPtr;
  if (!videoFormat) {
    SampleDataSharedPtr pSampleData;
    return pSampleData;
  }
  const IntegerSize sizeVideoFrame(videoFormat.sizePixels());
  if (sizeVideoFrame.width() <= 0) {
    SampleDataSharedPtr pSampleData;
    return pSampleData;
  }
  if (sizeVideoFrame.height() <= 0) {
    SampleDataSharedPtr pSampleData;
    return pSampleData;
  }
  const RGBImageType::point_t sizeImage(
      sizeVideoFrame.width(),
      sizeVideoFrame.height());
  RGBImageType rgbImage(sizeImage);
  SampleDataSharedPtr pSampleData(new RGBVideoFrame(rgbImage));
  return pSampleData;
}

} // VideoCapture
