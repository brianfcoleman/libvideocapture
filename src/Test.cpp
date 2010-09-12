#include "DirectShow.hpp"

int main() {
  using namespace VideoCapture;
  boost::shared_ptr<void> comInitializer = comInitializerSharedPtr();
  const QeditTypeLibrary qeditTypeLibrary;
  const IID iid_ISampleGrabber = qeditTypeLibrary.IID_ISampleGrabber();
  const IID iid_ISampleGrabberCB = qeditTypeLibrary.IID_ISampleGrabberCB();
  const CLSID clsid_SampleGrabber = qeditTypeLibrary.CLSID_SampleGrabber();
  const CLSID clsid_NullRenderer = qeditTypeLibrary.CLSID_NullRenderer();
}
