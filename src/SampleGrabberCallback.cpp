#include "SampleGrabberCallback.hpp"
#include "QeditTypeLibrary.hpp"

namespace VideoCapture {

template<
  typename RGBImageViewType,
  typename RGBPixelPtrType> static bool copyBufferIntoSample(
    const boost::uint8_t* buffer,
    SampleGrabberCallback::SampleType& sample);

SampleGrabberCallback::SampleGrabberCallback(
    const QeditTypeLibrary& qeditTypeLibrary,
    const RGBVideoFormat& videoFormat,
    const std::size_t maxCountAllocatedSamples)
    : boost::noncopyable(),
      m_refCount(1),
      m_IID_ISampleGrabberCB(qeditTypeLibrary.IID_ISampleGrabberCB()),
      m_sampleIndex(0),
      m_sampleAllocator(maxCountAllocatedSamples, videoFormat),
      m_outputSampleQueue(maxCountAllocatedSamples),
      m_recycledSampleQueue(maxCountAllocatedSamples) {

}

STDMETHODIMP_(ULONG) SampleGrabberCallback::AddRef() {
  return ++m_refCount;
}

STDMETHODIMP_(ULONG) SampleGrabberCallback::Release() {
  return --m_refCount;
}

STDMETHODIMP SampleGrabberCallback::QueryInterface(
    REFIID riid,
    void **ppvObject) {
  if (!ppvObject)
    return E_POINTER;

  if (riid == IID_IUnknown) {
    *ppvObject = static_cast<IUnknown*>(this);
    return S_OK;
  }

  if (riid == m_IID_ISampleGrabberCB) {
    *ppvObject = static_cast<ISampleGrabberCB*>(this);
    return S_OK;
  }

  return E_NOTIMPL;
}

STDMETHODIMP SampleGrabberCallback::SampleCB(
    double Time,
    IMediaSample *pSample) {
  return E_NOTIMPL;
}

STDMETHODIMP SampleGrabberCallback::BufferCB(
    double Time,
    BYTE *pBuffer,
    long BufferLen) {
  if (!pBuffer) {
    return S_FALSE;
  }

  if (BufferLen <= 0) {
    return S_FALSE;
  }

  RGBVideoFormat videoFormat = m_sampleAllocator.sampleFormat();
  if (!videoFormat) {
    return S_FALSE;
  }

  std::size_t sizeBytes = videoFormat.sizeBytes();
  if (BufferLen != sizeBytes) {
    return S_FALSE;
  }

  bool result = copyBufferIntoOutputSampleQueue(pBuffer);

  return result ? S_OK : S_FALSE;
}

SampleGrabberCallback::SampleType SampleGrabberCallback::allocateSample() {
  ++m_sampleIndex;
  SampleType sample(m_sampleAllocator.allocateSample(m_sampleIndex));
  if (!sample) {
    return sample;
  }
  return m_recycledSampleQueue.pop_back();
}

bool SampleGrabberCallback::copyBufferIntoOutputSampleQueue(
    boost::uint8_t* const buffer) {
  SampleType sample(allocateSample());
  if (!sample) {
    return false;
  }

  RGBVideoFormat videoFormat = sample.sampleFormat();
  if (!videoFormat) {
    return false;
  }

  bool result = false;
  RGBFormat rgbFormat = videoFormat.rgbFormat();
  switch (rgbFormat) {
    case RGB888:
      result = copyBufferIntoSample<rgb8c_view_t, rgb8c_ptr_t>(
          buffer,
          sample);
    case RGBA8888:
      result = copyBufferIntoSample<rgba8c_view_t, rgba8c_ptr_t>(
          buffer,
          sample);
    case BGR888:
      result = copyBufferIntoSample<bgr8c_view_t, bgr8c_ptr_t>(
          buffer,
          sample);
    case ABGR8888:
      result = copyBufferIntoSample<abgr8c_view_t, abgr8c_ptr_t>(
          buffer,
          sample);
    default:
      return false;
  }
  if (!result) {
    return false;
  }
  m_recycledSampleQueue.push_front(sample);
  return true;
}

template<
  typename RGBImageViewType,
  typename RGBPixelPtrType> static bool copyBufferIntoSample(
    const boost::uint8_t* buffer,
    SampleGrabberCallback::SampleType& sample) {
  if (!sample) {
    return false;
  }
  RGBVideoFormat videoFormat(sample.sampleFormat());
  IntegerSize sizePixels(videoFormat.sizePixels());
  if (sizePixels.width() <= 0) {
    return false;
  }
  if (sizePixels.height() <= 0) {
    return false;
  }
  std::size_t sizeRowBytes = videoFormat.sizeRowBytes();
  if (!sizeRowBytes) {
    return false;
  }
  RGBPixelPtrType pFirstPixel = reinterpret_cast<RGBPixelPtrType>(buffer);
  RGBImageViewType imageViewBuffer = boost::gil::interleaved_view(
      sizePixels.width(),
      sizePixels.height(),
      pFirstPixel,
      sizeRowBytes);
  SampleGrabberCallback::SampleType::SampleDataSharedPtr pSampleData(
      sample.sampleData());
  RGBVideoFrame::ImageViewType imageViewSample(pSampleData->imageView());
  imageViewSample = imageViewBuffer;
  return true;
}

} // VideoCapture

