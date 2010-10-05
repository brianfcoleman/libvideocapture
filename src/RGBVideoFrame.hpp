#ifndef VIDEO_CAPTURE_RGB_VIDEO_FRAME_H
#define VIDEO_CAPTURE_RGB_VIDEO_FRAME_H

#include <boost/gil/extension/dynamic_image/dynamic_image_all.hpp>
#include "boost/utility.hpp"
#include "RGBVideoFormat.hpp"
#include "RGBVideoFrameFactory.hpp"
#include "ByteBufferToRGBVideoFrameConverter.hpp"
#include "Sample.hpp"

namespace VideoCapture {

class RGBVideoFrame : private boost::noncopyable {
 public:
  typedef RGBVideoFrame SampleDataType;
  typedef Sample<SampleDataType> SampleType;
  typedef ByteBufferToRGBVideoFrameConverter<SampleType> RawSampleDataConverter;
  typedef RGBVideoFormat SampleFormatType;
  typedef RGBVideoFrameFactory SampleDataFactory;

  typedef boost::mpl::vector<
    rgb8_image_t,
    rgba8_image_t,
    bgr8_image_t,
    abgr8_image_t> SupportedImageType;
  typedef boost::gil::any_image<SupportedImageType> ImageType;

  typedef ImageType::view_t ImageViewType;
  typedef ImageType::const_view_t ConstantImageViewType;

  template<typename RGBImageType> explicit RGBVideoFrame(RGBImageType& rgbImage)
      : m_image(rgbImage) {

  }

  ImageViewType imageView();
  ConstantImageViewType constImageView() const;
 private:
  ImageType m_image;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_RGB_VIDEO_FRAME_H

