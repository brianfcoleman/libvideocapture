#ifndef VIDEO_CAPTURE_VIDEO_FORMAT_TYPES
#define VIDEO_CAPTURE_VIDEO_FORMAT_TYPES

#include "boost/gil/typedefs.hpp"
#include "IntegerSize.hpp"

namespace VideoCapture {
typedef enum RGBFormat {
  /**
   * Pixel interleaved format with each colour component stored in 8 bits.
   * The colour component order when iterating forward through the pixels
   * is R G B.
   * No alpha channel.
   */
  RGB888 = 0,
  /**
   * Pixel interleaved format with each colour component stored in 8 bits.
   * The colour component order when iterating forward through the pixels
   * is R G B A.
   * Includes an alpha channel.
   */
  RGBA8888 = 1,
  /**
   * Pixel interleaved format with each colour component stored in 8 bits.
   * The colour component order when iterating forward through the pixels
   * is B G R.
   * No alpha channel.
   */
  BGR888 = 2,
  /**
   * Pixel interleaved format with each colour component stored in 8 bits.
   * The colour component order when iterating forward through the pixels
   * is A B G R.
   * Includes an alpha channel.
   */
  ABGR8888 = 3,
  RGBNone = 4
} RGBFormat;

typedef boost::gil::rgb8_image_t rgb8_image_t;
typedef boost::gil::rgb8_image_t rgba8_image_t;
typedef boost::gil::rgb8_image_t bgr8_image_t;
typedef boost::gil::rgb8_image_t abgr8_image_t;

typedef boost::gil::rgb8_view_t rgb8_view_t;
typedef boost::gil::rgb8_view_t rgba8_view_t;
typedef boost::gil::rgb8_view_t bgr8_view_t;
typedef boost::gil::rgb8_view_t abgr8_view_t;

typedef boost::gil::rgb8c_view_t rgb8c_view_t;
typedef boost::gil::rgb8c_view_t rgba8c_view_t;
typedef boost::gil::rgb8c_view_t bgr8c_view_t;
typedef boost::gil::rgb8c_view_t abgr8c_view_t;

typedef boost::gil::rgb8_step_view_t rgb8_step_view_t;
typedef boost::gil::rgb8_step_view_t rgba8_step_view_t;
typedef boost::gil::rgb8_step_view_t bgr8_step_view_t;
typedef boost::gil::rgb8_step_view_t abgr8_step_view_t;

typedef boost::gil::rgb8c_step_view_t rgb8c_step_view_t;
typedef boost::gil::rgb8c_step_view_t rgba8c_step_view_t;
typedef boost::gil::rgb8c_step_view_t bgr8c_step_view_t;
typedef boost::gil::rgb8c_step_view_t abgr8c_step_view_t;

typedef boost::gil::rgb8_ptr_t rgb8_ptr_t;
typedef boost::gil::rgb8_ptr_t rgba8_ptr_t;
typedef boost::gil::rgb8_ptr_t bgr8_ptr_t;
typedef boost::gil::rgb8_ptr_t abgr8_ptr_t;

typedef boost::gil::rgb8c_ptr_t rgb8c_ptr_t;
typedef boost::gil::rgb8c_ptr_t rgba8c_ptr_t;
typedef boost::gil::rgb8c_ptr_t bgr8c_ptr_t;
typedef boost::gil::rgb8c_ptr_t abgr8c_ptr_t;

typedef boost::gil::rgb8_step_ptr_t rgb8_step_ptr_t;
typedef boost::gil::rgb8_step_ptr_t rgba8_step_ptr_t;
typedef boost::gil::rgb8_step_ptr_t bgr8_step_ptr_t;
typedef boost::gil::rgb8_step_ptr_t abgr8_step_ptr_t;

typedef boost::gil::rgb8c_step_ptr_t rgb8c_step_ptr_t;
typedef boost::gil::rgb8c_step_ptr_t rgba8c_step_ptr_t;
typedef boost::gil::rgb8c_step_ptr_t bgr8c_step_ptr_t;
typedef boost::gil::rgb8c_step_ptr_t abgr8c_step_ptr_t;

const boost::int32_t kAngleFullRotationDegrees = 360;
const boost::int32_t kAngleHalfRotationDegrees = 180;

bool isValidRGBFormat(const RGBFormat rgbFormat);
std::size_t bitsPerPixelForRGBFormat(const RGBFormat rgbFormat);
std::size_t sizeBytesForRGBFormat(
    const IntegerSize& sizePixels,
    const RGBFormat rgbFormat);
std::size_t sizeRowBytesForRGBFormat(
    const IntegerSize& sizePixels,
    const RGBFormat rgbFormat);

} // VideoCapture

#endif // VIDEO_CAPTURE_VIDEO_FORMAT_TYPES
