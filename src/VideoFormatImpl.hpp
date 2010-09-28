#ifndef VIDEOCAPTURE_VIDEO_FORMAT_IMPL_H
#define VIDEOCAPTURE_VIDEO_FORMAT_IMPL_H

#include <iostream>
#include "boost/utility.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "DirectShow.hpp"
#include "IntegerSize.hpp"
#include "VideoFormatTypes.hpp"

namespace VideoCapture {

class VideoFormatImpl : private boost::noncopyable {
 public:
  typedef boost::uuids::uuid Uuid;
  typedef boost::uuids::nil_generator NilUuidGenerator;
  VideoFormatImpl();
  VideoFormatImpl(
      const boost::shared_ptr<IAMStreamConfig>& pStreamConfig,
      const int index,
      const Uuid& uuid);
  bool isInitialized() const;
  bool isRGBFormat() const;
  const Uuid uuid() const;
  double framesPerSecond() const;
  IntegerSize sizePixels() const;
  boost::int32_t angleRotationDegrees() const;
  std::size_t bitsPerPixel() const;
  std::size_t sizeBytes() const;
  std::size_t sizeRowBytes() const;
  RGBFormat rgbFormat() const;
  bool VideoFormatImpl::setMediaTypeOfStream(
      const boost::shared_ptr<IAMStreamConfig>& pStreamConfig);
  operator bool() const {
      return isInitialized();
  }
 private:
  boost::shared_ptr<AM_MEDIA_TYPE> m_pMediaType;
  VIDEO_STREAM_CONFIG_CAPS m_streamCapabilities;
  bool m_isInitialized;
  Uuid m_uuid;
  double m_framesPerSecond;
  IntegerSize m_sizePixels;
  boost::int32_t m_angleRotationDegrees;
  boost::int32_t m_bitsPerPixel;
  bool m_uncompressedRGB;
  RGBFormat m_rgbFormat;
  bool initialize(const boost::shared_ptr<IAMStreamConfig>& pStreamConfig,
                  const int index);
  bool extractData();
  bool isVideoFormat() const;
};

std::ostream& operator<<(
    std::ostream& outputStream,
    const VideoFormatImpl& videoFormat);

} // VideoCapture

#endif // VIDEOCAPTURE_VIDEO_FORMAT_IMPL_H
