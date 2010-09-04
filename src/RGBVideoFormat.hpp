#ifndef VIDEO_CAPTURE_RGB_VIDEO_FORMAT_H
#define VIDEO_CAPTURE_RGB_VIDEO_FORMAT_H

#include "boost/shared_ptr.hpp"
#include "IntegerSize.hpp"
#include "VideoFormatTypes.hpp"

namespace VideoCapture {

class VideoFormatImpl;

class RGBVideoFormat {
 public:
  typedef boost::shared_ptr<VideoFormatImpl> ImplPtr;
  RGBVideoFormat();
  explicit RGBVideoFormat(const ImplPtr& pImpl);
  bool isInitialized() const;
  double framesPerSecond() const;
  IntegerSize sizePixels() const;
  Orientation orientation() const;
  std::size_t bitsPerPixel() const;
  std::size_t sizeBytes() const;
  std::size_t sizeRowBytes() const;
  RGBFormat rgbFormat() const;
  operator bool() const {
    return isInitialized();
  }
 private:
  ImplPtr m_pImpl;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_RGB_VIDEO_FORMAT_H
