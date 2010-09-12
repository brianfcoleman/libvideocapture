#ifndef VIDEO_CAPTURE_SAMPLE_GRABBER_CALLBACK_H
#define VIDEO_CAPTURE_SAMPLE_GRABBER_CALLBACK_H

#include <list>
#include "boost/utility.hpp"
#include "boost/cstdint.hpp"
#include "DirectShow.hpp"
// boost function must be included after DirectShow as DirectShow include
// boost bind and boost mem_fn with stdcall enabled. If boost function
// is included first, compilation will fail
#include "boost/function.hpp"
#include "ByteBuffer.hpp"

namespace VideoCapture {

class SampleGrabberCallback
    : public ISampleGrabberCB,
      private boost::noncopyable {
 public:
  typedef boost::function<void (ByteBuffer)> SampleProducerCallbackType;
  typedef std::list<SampleProducerCallbackType> SampleProducerCallbackList;
  typedef SampleProducerCallbackList::iterator SampleProducerCallbackIterator;
  SampleGrabberCallback(
      const IID& IID_ISampleGrabberCB,
      const SampleProducerCallbackList& sampleProducerCallbackList);
  STDMETHODIMP_(ULONG) AddRef();
  STDMETHODIMP_(ULONG) Release();
  STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);
  STDMETHODIMP SampleCB(double Time, IMediaSample *pSample);
  STDMETHODIMP BufferCB(double Time, BYTE *pBuffer, long BufferLen);
 private:
  boost::uint32_t m_refCount;
  IID m_IID_ISampleGrabberCB;
  SampleProducerCallbackList m_sampleProducerCallbackList;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_GRABBER_CALLBACK_H
