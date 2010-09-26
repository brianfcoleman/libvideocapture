#include "VideoCaptureDevice.hpp"
#include "VideoCaptureDeviceImpl.hpp"
#include "PimplUtilities.hpp"
#include "SynchronousMessage.hpp"

namespace VideoCapture {

const std::string VideoCaptureDevice::s_kEmptyString("");

VideoCaptureDevice::VideoCaptureDevice() {

}

VideoCaptureDevice::VideoCaptureDevice(
    const VideoCaptureDevice::ImplPtr& pImpl,
    const VideoCaptureDevice::MessageQueueType& messageQueue)
    : m_pImpl(pImpl),
      m_messageQueue(messageQueue) {

}

bool VideoCaptureDevice::isInitialized() const {
  return VideoCapture::isInitialized(m_pImpl);
}

VideoCaptureDevice::ImplPtr VideoCaptureDevice::lockImplPtr() const {
  return VideoCapture::lockImplPtr(m_pImpl);
}

std::string VideoCaptureDevice::name() const {
  typedef std::string ReturnType;
  typedef boost::function<ReturnType ()> MessageProcessorType;
  typedef MessageReturnValue<ReturnType> MessageReturnValueType;
  MessageReturnValueType messageReturnValue(s_kEmptyString);
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return messageReturnValue.returnValue();
  }
  MessageProcessorType messageProcessor(
      boost::bind<ReturnType>(
          &VideoCaptureDeviceImpl::name,
          pImpl));
  sendSynchronousMessage(
      messageProcessor,
      m_messageQueue,
      messageReturnValue);
  return messageReturnValue.returnValue();
}

bool VideoCaptureDevice::startCapturing(
    const VideoCaptureDevice::SampleProducerCallbackList&
    sampleProducerCallbackList) {
  typedef bool ReturnType;
  typedef boost::function<ReturnType ()> MessageProcessorType;
  typedef MessageReturnValue<ReturnType> MessageReturnValueType;
  MessageReturnValueType messageReturnValue(false);
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return messageReturnValue.returnValue();
  }
  MessageProcessorType messageProcessor(
      boost::bind<ReturnType>(
          &VideoCaptureDeviceImpl::startCapturing,
          pImpl,
          sampleProducerCallbackList));
  sendSynchronousMessage(
      messageProcessor,
      m_messageQueue,
      messageReturnValue);
  return messageReturnValue.returnValue();
}

bool VideoCaptureDevice::stopCapturing() {
  typedef bool ReturnType;
  typedef boost::function<ReturnType ()> MessageProcessorType;
  typedef MessageReturnValue<ReturnType> MessageReturnValueType;
  MessageReturnValueType messageReturnValue(false);
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return messageReturnValue.returnValue();
  }
  MessageProcessorType messageProcessor(
      boost::bind<ReturnType>(
          &VideoCaptureDeviceImpl::stopCapturing,
          pImpl));
  sendSynchronousMessage(
      messageProcessor,
      m_messageQueue,
      messageReturnValue);
  return messageReturnValue.returnValue();
}

double VideoCaptureDevice::countFramesCapturedPerSecond() const {
  typedef double ReturnType;
  typedef boost::function<ReturnType ()> MessageProcessorType;
  typedef MessageReturnValue<ReturnType> MessageReturnValueType;
  MessageReturnValueType messageReturnValue(0);
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return messageReturnValue.returnValue();
  }
  MessageProcessorType messageProcessor(
      boost::bind<ReturnType>(
          &VideoCaptureDeviceImpl::countFramesCapturedPerSecond,
          pImpl));
  sendSynchronousMessage(
      messageProcessor,
      m_messageQueue,
      messageReturnValue);
  return messageReturnValue.returnValue();
}

VideoCaptureDevice::RGBVideoFormatList
VideoCaptureDevice::videoFormatList() const {
  typedef RGBVideoFormatList ReturnType;
  typedef boost::function<ReturnType ()> MessageProcessorType;
  typedef MessageReturnValue<ReturnType> MessageReturnValueType;
  ReturnType emptyList;
  MessageReturnValueType messageReturnValue(emptyList);
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return messageReturnValue.returnValue();
  }
  MessageProcessorType messageProcessor(
      boost::bind<ReturnType>(
          &VideoCaptureDeviceImpl::videoFormatList,
          pImpl));
  sendSynchronousMessage(
      messageProcessor,
      m_messageQueue,
      messageReturnValue);
  return messageReturnValue.returnValue();
}

RGBVideoFormat VideoCaptureDevice::currentVideoFormat() const {
  typedef RGBVideoFormat ReturnType;
  typedef boost::function<ReturnType ()> MessageProcessorType;
  typedef MessageReturnValue<ReturnType> MessageReturnValueType;
  ReturnType uninitializedRGBVideoFormat;
  MessageReturnValueType messageReturnValue(uninitializedRGBVideoFormat);
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return messageReturnValue.returnValue();
  }
  MessageProcessorType messageProcessor(
      boost::bind<ReturnType>(
          &VideoCaptureDeviceImpl::currentVideoFormat,
          pImpl));
  sendSynchronousMessage(
      messageProcessor,
      m_messageQueue,
      messageReturnValue);
  return messageReturnValue.returnValue();
}

bool VideoCaptureDevice::setCurrentVideoFormat(
    const RGBVideoFormat& videoFormat) {
  typedef bool ReturnType;
  typedef boost::function<ReturnType ()> MessageProcessorType;
  typedef MessageReturnValue<ReturnType> MessageReturnValueType;
  MessageReturnValueType messageReturnValue(false);
  if (!videoFormat) {
    return messageReturnValue.returnValue();
  }
  ImplPtr pImpl(lockImplPtr());
  if (!pImpl) {
    return messageReturnValue.returnValue();
  }
  MessageProcessorType messageProcessor(
      boost::bind<ReturnType>(
          &VideoCaptureDeviceImpl::setCurrentVideoFormat,
          pImpl,
          videoFormat));
  sendSynchronousMessage(
      messageProcessor,
      m_messageQueue,
      messageReturnValue);
  return messageReturnValue.returnValue();
}

} // VideoCapture
