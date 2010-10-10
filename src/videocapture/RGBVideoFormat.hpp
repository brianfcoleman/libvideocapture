#ifndef VIDEO_CAPTURE_RGB_VIDEO_FORMAT_H
#define VIDEO_CAPTURE_RGB_VIDEO_FORMAT_H

#include "VideoFormat.hpp"

namespace VideoCapture {

class RGBVideoFormat {
 public:
  typedef VideoFormat::Uuid Uuid;
  typedef VideoFormat::NilUuidGenerator NilUuidGenerator;
  RGBVideoFormat();
  explicit RGBVideoFormat(const VideoFormat& videoFormat);
  RGBVideoFormat(
      const Uuid& uuid,
      const IntegerSize sizePixels,
      const boost::int32_t angleRotationDegrees,
      const RGBFormat rgbFormat);
  bool isInitialized() const;
  Uuid uuid() const;
  IntegerSize sizePixels() const;
  boost::int32_t angleRotationDegrees() const;
  std::size_t bitsPerPixel() const;
  std::size_t sizeBytes() const;
  std::size_t sizeRowBytes() const;
  RGBFormat rgbFormat() const;
  operator bool() const {
    return isInitialized();
  }
 private:
  bool initialize();
  bool m_isInitialized;
  Uuid m_uuid;
  IntegerSize m_sizePixels;
  boost::int32_t m_angleRotationDegrees;
  RGBFormat m_rgbFormat;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_RGB_VIDEO_FORMAT_H
