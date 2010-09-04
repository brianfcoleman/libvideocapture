#ifndef VIDEO_CAPTURE_RGB_VIDEO_FRAME_H
#define VIDEO_CAPTURE_RGB_VIDEO_FRAME_H

#include <boost/gil/extension/dynamic_image/dynamic_image_all.hpp>
#include "boost/utility.hpp"
#include "RGBVideoFormat.hpp"

namespace VideoCapture {

class RGBVideoFrame : private boost::noncopyable {
 public:
  typedef boost::mpl::vector<
    rgb8_image_t,
    rgba8_image_t,
    bgr8_image_t,
    abgr8_image_t> SupportedImageType;
  typedef boost::gil::any_image<SupportedImageType> ImageType;

  typedef boost::mpl::vector<
    rgb8_view_t,
    rgba8_view_t,
    bgr8_view_t,
    abgr8_view_t> SupportedImageViewType;
  typedef boost::gil::any_image_view<SupportedImageViewType> ImageViewType;

  typedef boost::mpl::vector<
    rgb8c_view_t,
    rgba8c_view_t,
    bgr8c_view_t,
    abgr8c_view_t> SupportedConstantImageViewType;
  typedef boost::gil::any_image_view<
    SupportedConstantImageViewType> ConstantImageViewType;

  typedef boost::mpl::vector<
    rgb8_step_view_t,
    rgba8_step_view_t,
    bgr8_step_view_t,
    abgr8_step_view_t> SupportedStepImageViewType;
  typedef boost::gil::any_image_view<
    SupportedStepImageViewType> StepImageViewType;

  explicit RGBVideoFrame(const RGBVideoFormat& videoFormat);
  ImageViewType imageView();
  ConstantImageViewType constImageView() const;
  bool isInitialized() const;
  operator bool() const {
    return isInitialized();
  }
 private:
  bool initialize(const RGBVideoFormat& videoFormat);
  ImageType m_image;
  bool m_isInitialized;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_RGB_VIDEO_FRAME_H

