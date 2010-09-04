#ifndef VIDEO_CAPTURE_SAMPLE_GRABBER_CALLBACK_H
#define VIDEO_CAPTURE_SAMPLE_GRABBER_CALLBACK_H

#include "boost/utility.hpp"
#include "boost/cstdint.hpp"
#include "DirectShow.hpp"
#include "BoundedBuffer.hpp"
#include "RGBVideoFrame.hpp"
#include "RGBVideoFrameFactory.hpp"
#include "RGBVideoFormat.hpp"
#include "Sample.hpp"
#include "SampleAllocator.hpp"

namespace VideoCapture {

class SampleGrabberCallback
    : public ISampleGrabberCB,
      private boost::noncopyable {
 public:
  typedef Sample<RGBVideoFrame, RGBVideoFormat> SampleType;
  typedef SampleAllocator<
    SampleType,
    RGBVideoFrameFactory,
    RGBVideoFormat> SampleAllocatorType;
  typedef bounded_buffer<SampleType> SampleQueueType;
  SampleGrabberCallback(
      const QeditTypeLibrary& qeditTypeLibrary,
      const RGBVideoFormat& videoFormat,
      const std::size_t maxCountAllocatedSamples);
  STDMETHODIMP_(ULONG) AddRef();
  STDMETHODIMP_(ULONG) Release();
  STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);
  STDMETHODIMP SampleCB(double Time, IMediaSample *pSample);
  STDMETHODIMP BufferCB(double Time, BYTE *pBuffer, long BufferLen);
 private:
  SampleType allocateSample();
  bool copyBufferIntoOutputSampleQueue(boost::uint8_t* const buffer);
  boost::uint32_t m_refCount;
  IID m_IID_ISampleGrabberCB;
  boost::uint32_t m_sampleIndex;
  SampleAllocatorType m_sampleAllocator;
  SampleQueueType m_outputSampleQueue;
  SampleQueueType m_recycledSampleQueue;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_GRABBER_CALLBACK_H
