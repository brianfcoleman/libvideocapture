#include "VideoFormatImpl.hpp"
#include "StreamFloatingPointPrecision.hpp"

namespace VideoCapture {

static const std::size_t kBitsPerByte = 8;

static std::size_t convertBitsToBytes(std::size_t countBits);

VideoFormatImpl::VideoFormatImpl()
    : boost::noncopyable(),
      m_isInitialized(false),
      m_framesPerSecond(0),
      m_orientation(OrientationNone),
      m_bitsPerPixel(0),
      m_uncompressedRGB(false),
      m_RGBFormat(RGBNone) {

}

VideoFormatImpl::VideoFormatImpl(
    const boost::shared_ptr<IAMStreamConfig>& streamConfig,
    const int& index)
    : boost::noncopyable(),
      m_isInitialized(false),
      m_framesPerSecond(0),
      m_orientation(OrientationNone),
      m_bitsPerPixel(0),
      m_uncompressedRGB(false),
      m_RGBFormat(RGBNone) {
  m_isInitialized = initialize(streamConfig, index);
}

bool VideoFormatImpl::isInitialized() const {
  return m_isInitialized;
}

bool VideoFormatImpl::isVideoFormat() const {
  if (!m_mediaType) {
    return false;
  }

  if (m_mediaType->majortype != MEDIATYPE_Video) {
    return false;
  }

  if (m_mediaType->formattype != FORMAT_VideoInfo) {
    return false;
  }

  if (m_mediaType->cbFormat != sizeof(VIDEOINFOHEADER)) {
    return false;
  }

  if (!m_mediaType->pbFormat) {
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

  if (m_RGBFormat == RGBNone) {
    return false;
  }

  return true;
}

bool VideoFormatImpl::initialize(
    const boost::shared_ptr<IAMStreamConfig>& streamConfig,
    const int& index) {
  AM_MEDIA_TYPE* pMediaType(0);
  HRESULT result;
  result = streamConfig->GetStreamCaps(
      index,
      &pMediaType,
      reinterpret_cast<boost::uint8_t*>(&m_streamCapabilities));

  if (FAILED(result)) {
    return false;
  }
  if (!pMediaType) {
    return false;
  }

  m_mediaType = mediaTypeSharedPtr(pMediaType);

  if (!m_mediaType) {
    return false;
  }

  return extractData();
}

bool VideoFormatImpl::extractData() {
  if (!isVideoFormat()) {
    return false;
  }

  VIDEOINFOHEADER& videoHeader(
      *(reinterpret_cast<VIDEOINFOHEADER*>(m_mediaType->pbFormat)));
  BITMAPINFOHEADER& bmiHeader(videoHeader.bmiHeader);

  double timePerFrameMs =
      static_cast<double>(videoHeader.AvgTimePerFrame) / 10000.0;
  m_framesPerSecond = 1000.0 / timePerFrameMs;

  boost::int32_t widthPixels = bmiHeader.biWidth;
  boost::int32_t heightPixels = bmiHeader.biHeight;
  if (heightPixels >= 0) {
    m_orientation = OrientationInverted;
  } else {
    m_orientation = OrientationNonInverted;
    heightPixels = -heightPixels;
  }
  m_sizePixels = IntegerSize(widthPixels, heightPixels);

  m_bitsPerPixel = bmiHeader.biBitCount;

  if (bmiHeader.biCompression == BI_RGB) {
    m_uncompressedRGB = true;
  } else {
    m_uncompressedRGB = false;
  }

  if (m_mediaType->subtype == MEDIASUBTYPE_RGB24) {
    m_RGBFormat = RGB888;
  }

  if (m_mediaType->subtype == MEDIASUBTYPE_RGB32) {
    m_RGBFormat = RGBA8888;
  }

  return true;
}

double VideoFormatImpl::framesPerSecond() const {
  return m_framesPerSecond;
}

IntegerSize VideoFormatImpl::sizePixels() const {
  return m_sizePixels;
}

Orientation VideoFormatImpl::orientation() const {
  return m_orientation;
}

std::size_t VideoFormatImpl::bitsPerPixel() const {
  return m_bitsPerPixel;
}

std::size_t VideoFormatImpl::sizeBytes() const {
  if (!isInitialized()) {
    return 0;
  }

  if (m_sizePixels.width() <= 0) {
    return 0;
  }

  if (m_sizePixels.height() <= 0) {
    return 0;
  }

  if (m_bitsPerPixel == 0) {
    return 0;
  }

  std::size_t countPixels = m_sizePixels.width() * m_sizePixels.height();
  std::size_t sizeBits = m_bitsPerPixel * countPixels;

  std::size_t sizeBytes = convertBitsToBytes(sizeBits);
  return sizeBytes;
}

std::size_t VideoFormatImpl::sizeRowBytes() const {
  if (!isInitialized()) {
    return false;
  }

  if (m_sizePixels.width() <= 0) {
    return 0;
  }

  if (m_sizePixels.height() <= 0) {
    return 0;
  }

  if (m_bitsPerPixel == 0) {
    return 0;
  }

  std::size_t sizeRowBits = m_bitsPerPixel * m_sizePixels.width();
  std::size_t sizeRowBytes = convertBitsToBytes(sizeRowBits);
  return sizeRowBytes;
}

RGBFormat VideoFormatImpl::rgbFormat() const {
  return m_RGBFormat;
}

bool VideoFormatImpl::setMediaTypeOfStream(
    const boost::shared_ptr<IAMStreamConfig>& streamConfig) {
  if (!isInitialized()) {
    return false;
  }

  if (!isRGBFormat()) {
    return false;
  }

  if (!streamConfig) {
    return false;
  }

  HRESULT result;
  result = streamConfig->SetFormat(m_mediaType.get());
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
  switch (videoFormat.orientation()) {
    case OrientationNonInverted:
      outputStream << "orientation: OrientationNonInverted" << std::endl;
      break;
    case OrientationInverted:
      outputStream << "orientation: OrientationInverted" << std::endl;
      break;
    default:
      break;
  }
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

static std::size_t convertBitsToBytes(std::size_t countBits) {
  if ((countBits % kBitsPerByte) != 0) {
    return 0;
  }

  std::size_t countBytes = countBits / kBitsPerByte;
  return countBytes;
}

} // VideoCapture
