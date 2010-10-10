#include "RGBVideoFormat.hpp"

namespace VideoCapture {

RGBVideoFormat::RGBVideoFormat()
    : m_isInitialized(false),
      m_uuid(NilUuidGenerator()()),
      m_angleRotationDegrees(0),
      m_rgbFormat(RGBNone) {

}

RGBVideoFormat::RGBVideoFormat(const VideoFormat& videoFormat)
    : m_isInitialized(videoFormat),
      m_uuid(videoFormat.uuid()),
      m_sizePixels(videoFormat.sizePixels()),
      m_angleRotationDegrees(videoFormat.angleRotationDegrees()),
      m_rgbFormat(videoFormat.rgbFormat()) {

}

RGBVideoFormat::RGBVideoFormat(
    const RGBVideoFormat::Uuid& uuid,
    const IntegerSize sizePixels,
    const boost::int32_t angleRotationDegrees,
    const RGBFormat rgbFormat)
    : m_isInitialized(false),
      m_uuid(uuid),
      m_sizePixels(sizePixels),
      m_angleRotationDegrees(angleRotationDegrees % kAngleFullRotationDegrees),
      m_rgbFormat(rgbFormat) {
  m_isInitialized = initialize();
}

bool RGBVideoFormat::initialize() {
  if (m_uuid.is_nil()) {
    return false;
  }

  if (!isValidRGBFormat(m_rgbFormat)) {
    return false;
  }

  if (m_sizePixels.width() <= 0) {
    return false;
  }

  if (m_sizePixels.height() <= 0) {
    return false;
  }

  return true;
}

bool RGBVideoFormat::isInitialized() const {
  return m_isInitialized;
}

RGBVideoFormat::Uuid RGBVideoFormat::uuid() const {
  return m_uuid;
}

IntegerSize RGBVideoFormat::sizePixels() const {
  return m_sizePixels;
}

boost::int32_t RGBVideoFormat::angleRotationDegrees() const {
  return m_angleRotationDegrees;
}

std::size_t RGBVideoFormat::bitsPerPixel() const {
  if (!isInitialized()) {
    return 0;
  }

  return bitsPerPixelForRGBFormat(m_rgbFormat);
}

std::size_t RGBVideoFormat::sizeBytes() const {
  if (!isInitialized()) {
    return 0;
  }

  return sizeBytesForRGBFormat(m_sizePixels, m_rgbFormat);
}

std::size_t RGBVideoFormat::sizeRowBytes() const {
  if (!isInitialized()) {
    return 0;
  }

  return sizeRowBytesForRGBFormat(m_sizePixels, m_rgbFormat);
}

RGBFormat RGBVideoFormat::rgbFormat() const {
  return m_rgbFormat;
}

} // VideoCapture
