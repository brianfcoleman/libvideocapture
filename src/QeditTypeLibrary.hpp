#ifndef VIDEO_CAPTURE_QEDIT_TYPE_LIBRARY
#define VIDEO_CAPTURE_QEDIT_TYPE_LIBRARY

#include "boost/shared_ptr.hpp"
#include "oaidl.h"

namespace VideoCapture {

class QeditTypeLibraryImpl;

class QeditTypeLibrary {
public:
  QeditTypeLibrary();
  IID IID_ISampleGrabber() const;
  IID IID_ISampleGrabberCB() const;
  CLSID CLSID_SampleGrabber() const;
  operator bool() const {
    return isInitialized();
  }
private:
  bool isInitialized() const;
  boost::shared_ptr<QeditTypeLibraryImpl> m_pImpl;
};
  
} // VideoCapture

#endif // VIDEO_CAPTURE_QEDIT_TYPE_LIBRARY
