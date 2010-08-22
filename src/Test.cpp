#include "DirectShow.h"

int main() {
  using namespace VideoCapture;
  boost::shared_ptr<void> comInitializer = comInitializerSharedPtr();
  const QeditTypeLibrary qeditTypeLibrary;
  IID iid_ISampleGrabber = qeditTypeLibrary.IID_ISampleGrabber();
  IID iid_ISampleGrabberCB = qeditTypeLibrary.IID_ISampleGrabberCB();
  CLSID clsid_SampleGrabber = qeditTypeLibrary.CLSID_SampleGrabber();
}
