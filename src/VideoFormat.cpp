#include "VideoFormat.hpp"
#include "VideoFormatImpl.hpp"
#include "PimplUtilities.hpp"

namespace VideoCapture {

VideoFormat::VideoFormat() {

}

VideoFormat::VideoFormat(const VideoFormat::ImplPtr& pImpl)
    : m_pImpl(pImpl) {

}

bool VideoFormat::isInitialized() const {
  return VideoCapture::isInitialized(m_pImpl);
}

VideoFormat::ImplPtr VideoFormat::lockImplPtr() const {
  return VideoCapture::lockImplPtr(m_pImpl);
}

VideoFormat::Uuid VideoFormat::uuid() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    NilUuidGenerator generator;
    Uuid nilUuid(generator());
    return nilUuid;
  }
  return pImpl->uuid();
}

double VideoFormat::framesPerSecond() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return 0;
  }
  return pImpl->framesPerSecond();
}

IntegerSize VideoFormat::sizePixels() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    IntegerSize zeroSize;
    return zeroSize;
  }
  return pImpl->sizePixels();
}

boost::int32_t VideoFormat::angleRotationDegrees() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return 0;
  }
  return pImpl->angleRotationDegrees();
}

std::size_t VideoFormat::bitsPerPixel() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return 0;
  }
  return pImpl->bitsPerPixel();
}

std::size_t VideoFormat::sizeBytes() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return 0;
  }
  return pImpl->sizeBytes();
}

std::size_t VideoFormat::sizeRowBytes() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return 0;
  }
  return pImpl->sizeRowBytes();
}

RGBFormat VideoFormat::rgbFormat() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return RGBNone;
  }
  return pImpl->rgbFormat();
}

} // VideoCapture

