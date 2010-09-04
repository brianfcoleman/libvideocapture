#include "RGBVideoFormat.hpp"
#include "VideoFormatImpl.hpp"

namespace VideoCapture {

RGBVideoFormat::RGBVideoFormat() {

}

RGBVideoFormat::RGBVideoFormat(const ImplPtr& pImpl)
    : m_pImpl(pImpl) {

}

bool RGBVideoFormat::isInitialized() const {
  if (!m_pImpl) {
    return false;
  }
  return m_pImpl->isInitialized();
}

double RGBVideoFormat::framesPerSecond() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pImpl->framesPerSecond();
}

IntegerSize RGBVideoFormat::sizePixels() const {
  if (!isInitialized()) {
    IntegerSize zeroSize;
    return zeroSize;
  }
  return m_pImpl->sizePixels();
}

Orientation RGBVideoFormat::orientation() const {
  if (!isInitialized()) {
    return OrientationNone;
  }
  return m_pImpl->orientation();
}

std::size_t RGBVideoFormat::bitsPerPixel() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pImpl->bitsPerPixel();
}

std::size_t RGBVideoFormat::sizeBytes() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pImpl->sizeBytes();
}

std::size_t RGBVideoFormat::sizeRowBytes() const {
  if (!isInitialized()) {
    return 0;
  }
  return m_pImpl->sizeRowBytes();
}

RGBFormat RGBVideoFormat::rgbFormat() const {
  if (!isInitialized()) {
    return RGBNone;
  }
  return m_pImpl->rgbFormat();
}

} // VideoCapture

