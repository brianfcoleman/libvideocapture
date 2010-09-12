#include "RGBVideoFrame.hpp"

namespace VideoCapture {

template<typename RGBImageType>
static bool createRGBImageAndAssignToDestinationImage(
    const RGBVideoFormat& videoFormat,
    RGBVideoFrame::ImageType& destinationImage);

RGBVideoFrame::RGBVideoFrame(const RGBVideoFormat& videoFormat)
    : boost::noncopyable(),
      m_isInitialized(false) {
  m_isInitialized = initialize(videoFormat);
}

RGBVideoFrame::ImageViewType RGBVideoFrame::imageView() {
  ImageViewType view(boost::gil::view(m_image));
  return view;
}

RGBVideoFrame::ConstantImageViewType RGBVideoFrame::constImageView() const {
  ConstantImageViewType view(boost::gil::const_view(m_image));
  return view;
}

bool RGBVideoFrame::isInitialized() const {
  return m_isInitialized;
}

bool RGBVideoFrame::initialize(const RGBVideoFormat& videoFormat) {
  if (!videoFormat) {
    return false;
  }
  RGBFormat rgbFormat = videoFormat.rgbFormat();
  switch (rgbFormat) {
    case RGB888:
      return createRGBImageAndAssignToDestinationImage<rgb8_image_t>(
          videoFormat,
          m_image);
    case BGR888:
      return createRGBImageAndAssignToDestinationImage<bgr8_image_t>(
          videoFormat,
          m_image);
    case RGBA8888:
      return createRGBImageAndAssignToDestinationImage<rgba8_image_t>(
          videoFormat,
          m_image);
    case ABGR8888:
      return createRGBImageAndAssignToDestinationImage<abgr8_image_t>(
          videoFormat,
          m_image);
    default:
      return false;
  }
}

template<typename RGBImageType>
static bool createRGBImageAndAssignToDestinationImage(
    const RGBVideoFormat& videoFormat,
    RGBVideoFrame::ImageType& destinationImage) {
  if (!videoFormat) {
    return false;
  }
  const IntegerSize sizeVideoFrame(videoFormat.sizePixels());
  if (sizeVideoFrame.width() <= 0) {
    return false;
  }
  if (sizeVideoFrame.height() <= 0) {
    return false;
  }
  const RGBImageType::point_t sizeImage(
      sizeVideoFrame.width(),
      sizeVideoFrame.height());
  RGBImageType rgbImage(sizeImage);
  destinationImage = rgbImage;
  return false;
}

} // VideoCapture
