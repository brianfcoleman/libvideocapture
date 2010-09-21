#include "VideoCaptureDevice.hpp"
#include "VideoCaptureDeviceImpl.hpp"
#include "PimplUtilities.hpp"

namespace VideoCapture {

const std::string VideoCaptureDevice::s_kEmptyString("");

VideoCaptureDevice::VideoCaptureDevice() {

}

VideoCaptureDevice::VideoCaptureDevice(const VideoCaptureDevice::ImplPtr& pImpl)
    : m_pImpl(pImpl) {

}

bool VideoCaptureDevice::isInitialized() const {
  return VideoCapture::isInitialized(m_pImpl);
}

VideoCaptureDevice::ImplPtr VideoCaptureDevice::lockImplPtr() const {
  return VideoCapture::lockImplPtr(m_pImpl);
}

const std::string VideoCaptureDevice::name() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return s_kEmptyString;
  }
  return pImpl->name();
}

bool VideoCaptureDevice::startCapturing(
    const VideoCaptureDevice::SampleProducerCallbackList&
    sampleProducerCallbackList) {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return false;
  }
  return pImpl->startCapturing(sampleProducerCallbackList);
}

bool VideoCaptureDevice::stopCapturing() {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return false;
  }
  return pImpl->stopCapturing();
}

double VideoCaptureDevice::countFramesCapturedPerSecond() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return 0;
  }
  return pImpl->countFramesCapturedPerSecond();
}

const VideoCaptureDevice::RGBVideoFormatList
VideoCaptureDevice::videoFormatList() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    RGBVideoFormatList emptyList;
    return emptyList;
  }
  return pImpl->videoFormatList();
}

const RGBVideoFormat VideoCaptureDevice::currentVideoFormat() const {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    RGBVideoFormat videoFormat;
    return videoFormat;
  }
  return pImpl->currentVideoFormat();
}

bool VideoCaptureDevice::setCurrentVideoFormat(
    const RGBVideoFormat& videoFormat) {
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return false;
  }
  return pImpl->setCurrentVideoFormat(videoFormat);
}

} // VideoCapture
