#ifndef VIDEOCAPTURE_VIDEO_FORMAT_IMPL_H
#define VIDEOCAPTURE_VIDEO_FORMAT_IMPL_H

#include <iostream>
#include "boost/utility.hpp"
#include "boost/cstdint.hpp"
#include "boost/shared_ptr.hpp"
#include "IntegerSize.h"
#include "StreamFloatingPointPrecision.h"
#include "DirectShow.h"

namespace VideoCapture {

typedef enum RGBFormat {
  RGB24 = 0,
  RGB32 = 1,
  RGBNone = 2
} RGBFormat;

typedef enum Orientation {
  OrientationNonInverted = 0,
  OrientationInverted = 1
}
Orientation;

class VideoFormatImpl : private boost::noncopyable {
 public:
  VideoFormatImpl();
  VideoFormatImpl(
      const boost::shared_ptr<IAMStreamConfig>& streamConfig,
      const int& index);
  bool isInitialized() const;
  bool isRGBFormat() const;
  double framesPerSecond() const;
  IntegerSize sizePixels() const;
  Orientation orientation() const;
  boost::int32_t bitsPerPixel() const;
  RGBFormat rgbFormat() const;
  bool VideoFormatImpl::setVideoFormatOfStream(
      const boost::shared_ptr<IAMStreamConfig>& streamConfig);
  operator bool() const {
      return isInitialized();
  }
 private:
  boost::shared_ptr<AM_MEDIA_TYPE> m_mediaType;
  VIDEO_STREAM_CONFIG_CAPS m_streamCapabilities;
  bool m_isInitialized;
  double m_framesPerSecond;
  IntegerSize m_sizePixels;
  Orientation m_orientation;
  boost::int32_t m_bitsPerPixel;
  bool m_uncompressedRGB;
  RGBFormat m_RGBFormat;
  bool initialize(const boost::shared_ptr<IAMStreamConfig>& streamConfig,
                  const int& index);
  bool extractData();
  bool isVideoFormat() const;
};

std::ostream& operator<<(
    std::ostream& outputStream,
    const VideoFormatImpl& videoFormat);

} // VideoCapture

#endif // VIDEOCAPTURE_VIDEO_FORMAT_IMPL_H
