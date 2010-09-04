#ifndef VIDEOCAPTURE_VIDEO_FORMAT_IMPL_H
#define VIDEOCAPTURE_VIDEO_FORMAT_IMPL_H

#include <iostream>
#include "boost/utility.hpp"
#include "boost/shared_ptr.hpp"
#include "DirectShow.hpp"
#include "IntegerSize.hpp"
#include "VideoFormatTypes.hpp"

namespace VideoCapture {

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
  std::size_t bitsPerPixel() const;
  std::size_t sizeBytes() const;
  std::size_t sizeRowBytes() const;
  RGBFormat rgbFormat() const;
  bool VideoFormatImpl::setMediaTypeOfStream(
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
