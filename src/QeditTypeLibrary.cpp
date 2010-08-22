#include "QeditTypeLibrary.h"
#include "QeditTypeLibraryImpl.h"

namespace VideoCapture {

QeditTypeLibrary::QeditTypeLibrary() :
  m_pImpl(new QeditTypeLibraryImpl()) {

}

IID QeditTypeLibrary::IID_ISampleGrabber() const {
  return m_pImpl->IID_ISampleGrabber();
}

IID QeditTypeLibrary::IID_ISampleGrabberCB() const {
  return m_pImpl->IID_ISampleGrabberCB();
}

CLSID QeditTypeLibrary::CLSID_SampleGrabber() const {
  return m_pImpl->CLSID_SampleGrabber();
}

bool QeditTypeLibrary::isInitialized() const {
  if (!m_pImpl) {
    return false;
  }
  return m_pImpl.get();
}

}
