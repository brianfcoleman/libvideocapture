#include "QeditTypeLibrary.hpp"
#include "QeditTypeLibraryImpl.hpp"

namespace VideoCapture {

QeditTypeLibrary::QeditTypeLibrary() :
  m_pImpl(new QeditTypeLibraryImpl()) {

}

const IID QeditTypeLibrary::IID_ISampleGrabber() const {
  return m_pImpl->IID_ISampleGrabber();
}

const IID QeditTypeLibrary::IID_ISampleGrabberCB() const {
  return m_pImpl->IID_ISampleGrabberCB();
}

const CLSID QeditTypeLibrary::CLSID_SampleGrabber() const {
  return m_pImpl->CLSID_SampleGrabber();
}

const CLSID QeditTypeLibrary::CLSID_NullRenderer() const {
  return m_pImpl->CLSID_NullRenderer();
}

bool QeditTypeLibrary::isInitialized() const {
  if (!m_pImpl) {
    return false;
  }
  return m_pImpl.get();
}

}
