#include "RGBVideoFormat.hpp"
#include "VideoFormatImpl.hpp"
#include "PimplUtilities.hpp"

namespace VideoCapture {

RGBVideoFormat::RGBVideoFormat() {

}

RGBVideoFormat::RGBVideoFormat(const RGBVideoFormat::ImplPtr& pImpl)
    : m_pImpl(pImpl) {

}

bool RGBVideoFormat::isInitialized() const {
  return VideoCapture::isInitialized(m_pImpl);
}

RGBVideoFormat::ImplPtr RGBVideoFormat::lockImplPtr() const {
  return VideoCapture::lockImplPtr(m_pImpl);
}

const RGBVideoFormat::Uuid RGBVideoFormat::uuid() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    NilUuidGenerator generator;
    Uuid nilUuid(generator());
    return nilUuid;
  }
  return pImpl->uuid();
}

double RGBVideoFormat::framesPerSecond() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return 0;
  }
  return pImpl->framesPerSecond();
}

IntegerSize RGBVideoFormat::sizePixels() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    IntegerSize zeroSize;
    return zeroSize;
  }
  return pImpl->sizePixels();
}

Orientation RGBVideoFormat::orientation() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return OrientationNone;
  }
  return pImpl->orientation();
}

std::size_t RGBVideoFormat::bitsPerPixel() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return 0;
  }
  return pImpl->bitsPerPixel();
}

std::size_t RGBVideoFormat::sizeBytes() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return 0;
  }
  return pImpl->sizeBytes();
}

std::size_t RGBVideoFormat::sizeRowBytes() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return 0;
  }
  return pImpl->sizeRowBytes();
}

RGBFormat RGBVideoFormat::rgbFormat() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return RGBNone;
  }
  return pImpl->rgbFormat();
}

} // VideoCapture

