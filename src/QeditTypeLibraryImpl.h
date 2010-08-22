#ifndef QEDIT_TYPE_LIBRARY_IMPL
#define QEDIT_TYPE_LIBRARY_IMPL

#include "TypeLibrary.h"

namespace VideoCapture {

class QeditTypeLibraryImpl :
  public TypeLibrary {
public:
  QeditTypeLibraryImpl();
  IID IID_ISampleGrabber() const;
  IID IID_ISampleGrabberCB() const;
  CLSID CLSID_SampleGrabber() const;
};

}

#endif // QEDIT_TYPE_LIBRARY_IMPL