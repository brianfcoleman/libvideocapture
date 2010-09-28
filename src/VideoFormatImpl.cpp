#include "VideoFormatImpl.hpp"
#include "StreamFloatingPointPrecision.hpp"

namespace VideoCapture {

VideoFormatImpl::VideoFormatImpl()
    : m_isInitialized(false),
      m_uuid(NilUuidGenerator()()),
      m_framesPerSecond(0),
      m_angleRotationDegrees(0),
      m_bitsPerPixel(0),
      m_uncompressedRGB(false),
      m_rgbFormat(RGBNone) {

}

VideoFormatImpl::VideoFormatImpl(
    const boost::shared_ptr<IAMStreamConfig>& pStreamConfig,
    const int index,
    const VideoFormatImpl::Uuid& uuid)
    : m_isInitialized(false),
      m_uuid(uuid),
      m_framesPerSecond(0),
      m_angleRotationDegrees(0),
      m_bitsPerPixel(0),
      m_uncompressedRGB(false),
      m_rgbFormat(RGBNone) {
  m_isInitialized = initialize(pStreamConfig, index);
}

bool VideoFormatImpl::isInitialized() const {
  return m_isInitialized;
}

bool VideoFormatImpl::isVideoFormat() const {
  if (!m_pMediaType) {
    return false;
  }

  if (m_pMediaType->majortype != MEDIATYPE_Video) {
    return false;
  }

  if (m_pMediaType->formattype != FORMAT_VideoInfo) {
    return false;
  }

  if (m_pMediaType->cbFormat != sizeof(VIDEOINFOHEADER)) {
    return false;
  }

  if (!m_pMediaType->pbFormat) {
    return false;
  }

  return true;
}

bool VideoFormatImpl::isRGBFormat() const {
  if (!isInitialized()) {
    return false;
  }

  if (!isVideoFormat()) {
    return false;
  }

  if (!m_uncompressedRGB) {
    return false;
  }

  if (m_rgbFormat == RGBNone) {
    return false;
  }

  return true;
}

bool VideoFormatImpl::initialize(
    const boost::shared_ptr<IAMStreamConfig>& pStreamConfig,
    const int index) {
  if (m_uuid.is_nil()) {
    return false;
  }

  AM_MEDIA_TYPE* pMediaType = 0;
  HRESULT result;
  result = pStreamConfig->GetStreamCaps(
      index,
      &pMediaType,
      reinterpret_cast<boost::uint8_t*>(&m_streamCapabilities));
  if (FAILED(result)) {
    return false;
  }
  m_pMediaType = mediaTypeSharedPtr(pMediaType);
  if (!m_pMediaType) {
    return false;
  }

  if (!extractData()) {
    return false;
  }

  return true;
}

bool VideoFormatImpl::extractData() {
  if (!isVideoFormat()) {
    return false;
  }

  VIDEOINFOHEADER& videoHeader(
      *(reinterpret_cast<VIDEOINFOHEADER*>(m_pMediaType->pbFormat)));
  BITMAPINFOHEADER& bmiHeader(videoHeader.bmiHeader);

  double timePerFrameMs =
      static_cast<double>(videoHeader.AvgTimePerFrame) / 10000.0;
  m_framesPerSecond = 1000.0 / timePerFrameMs;

  boost::int32_t widthPixels = bmiHeader.biWidth;
  boost::int32_t heightPixels = bmiHeader.biHeight;
  if (heightPixels >= 0) {
    m_angleRotationDegrees = kAngleHalfRotationDegrees;
  } else {
    m_angleRotationDegrees = 0;
    heightPixels = -heightPixels;
  }
  m_sizePixels = IntegerSize(widthPixels, heightPixels);

  m_bitsPerPixel = bmiHeader.biBitCount;

  if (bmiHeader.biCompression == BI_RGB) {
    m_uncompressedRGB = true;
  } else {
    m_uncompressedRGB = false;
  }

  if (m_pMediaType->subtype == MEDIASUBTYPE_RGB24) {
    m_rgbFormat = RGB888;
  }

  if (m_pMediaType->subtype == MEDIASUBTYPE_RGB32) {
    m_rgbFormat = RGBA8888;
  }

  return true;
}

const VideoFormatImpl::Uuid VideoFormatImpl::uuid() const {
  return m_uuid;
}

double VideoFormatImpl::framesPerSecond() const {
  return m_framesPerSecond;
}

IntegerSize VideoFormatImpl::sizePixels() const {
  return m_sizePixels;
}

boost::int32_t VideoFormatImpl::angleRotationDegrees() const {
  return m_angleRotationDegrees;
}

std::size_t VideoFormatImpl::bitsPerPixel() const {
  return m_bitsPerPixel;
}

std::size_t VideoFormatImpl::sizeBytes() const {
  if (!isInitialized()) {
    return 0;
  }

  return sizeBytesForRGBFormat(m_sizePixels, m_rgbFormat);
}

std::size_t VideoFormatImpl::sizeRowBytes() const {
  if (!isInitialized()) {
    return false;
  }

  return sizeRowBytesForRGBFormat(m_sizePixels, m_rgbFormat);
}

RGBFormat VideoFormatImpl::rgbFormat() const {
  return m_rgbFormat;
}

bool VideoFormatImpl::setMediaTypeOfStream(
    const boost::shared_ptr<IAMStreamConfig>& pStreamConfig) {
  if (!isInitialized()) {
    return false;
  }

  if (!isRGBFormat()) {
    return false;
  }

  if (!pStreamConfig) {
    return false;
  }

  HRESULT result;
  result = pStreamConfig->SetFormat(m_pMediaType.get());
  return SUCCEEDED(result);
}

std::ostream& operator<<(
    std::ostream& outputStream,
    const VideoFormatImpl& videoFormat) {
  StreamFloatingPointPrecision floatingPointPrecision(
      outputStream,
      StreamFloatingPointPrecision::s_kPrecision);
  outputStream << "VideoFormatImpl" << std::endl;
  outputStream << "framesPerSecond: " << videoFormat.framesPerSecond()
               << std::endl;
  outputStream << "sizePixels: " << videoFormat.sizePixels();
  outputStream << "angleRotationDegrees: " << videoFormat.angleRotationDegrees();
  outputStream << "bitsPerPixel: " << videoFormat.bitsPerPixel() << std::endl;
  switch (videoFormat.rgbFormat()) {
    case RGB888:
      outputStream << "RGBFormat: RGB888" << std::endl;
      break;
    case RGBA8888:
      outputStream << "RGBFormat: RGBA8888" << std::endl;
      break;
    default:
      break;
  }
  return outputStream;
}

} // VideoCapture
