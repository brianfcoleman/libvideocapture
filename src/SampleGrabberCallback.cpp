#include "SampleGrabberCallback.hpp"
#include "PimplUtilities.hpp"

#ifdef DEBUG
#include <iostream>
#endif

namespace VideoCapture {

SampleGrabberCallback::SampleGrabberCallback(
    const IID& IID_ISampleGrabberCB,
    const SampleGrabberCallback::SampleProducerCallbackSet&
    sampleProducerCallbackSet)
    : m_refCount(1),
      m_IID_ISampleGrabberCB(IID_ISampleGrabberCB),
      m_sampleProducerCallbackSet(sampleProducerCallbackSet) {

}

STDMETHODIMP_(ULONG) SampleGrabberCallback::AddRef() {
  return ++m_refCount;
}

STDMETHODIMP_(ULONG) SampleGrabberCallback::Release() {
  if (!m_refCount) {
    return 0;
  }
  return --m_refCount;
}

STDMETHODIMP SampleGrabberCallback::QueryInterface(
    REFIID riid,
    void **ppvObject) {
  if (!ppvObject) {
    return E_POINTER;
  }

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
#ifdef DEBUG
  std::cout << "SampleGrabberCallback::SampleCB" << std::endl;
#endif
  return E_NOTIMPL;
}

STDMETHODIMP SampleGrabberCallback::BufferCB(
    double Time,
    BYTE *pBuffer,
    long BufferLen) {
#ifdef DEBUG
  std::cout << "SampleGrabberCallback::BufferCB" << std::endl;
#endif
  if (!pBuffer) {
    return S_FALSE;
  }

  if (BufferLen <= 0) {
    return S_FALSE;
  }

  ByteBuffer buffer(pBuffer, BufferLen);

  for(SampleProducerCallbackIterator iterator(
          m_sampleProducerCallbackSet.begin());
      iterator != m_sampleProducerCallbackSet.end();
      ++iterator) {
    SampleProducerCallbackSharedPtr pCallback(*iterator);
    if (!pCallback) {
      continue;
    }
    pCallback->operator()(buffer);
  }

  return S_OK;
}

} // VideoCapture

