#include "QeditTypeLibraryImpl.hpp"

namespace VideoCapture {

static const std::wstring kTypeLibraryName(L"qedit.dll");

static const std::wstring kIID_ISampleGrabberName(L"ISampleGrabber");

static const std::wstring
kIID_ISampleGrabberCBName(L"ISampleGrabberCB");

static const std::wstring kCLSID_SampleGrabberName(L"SampleGrabber");

static const std::vector<const std::wstring> interfaceIdentifierNames();

static const std::vector<const std::wstring> classIdentifierNames();

static const std::vector<const std::wstring> interfaceIdentifierNames() {
  std::vector<const std::wstring> names;
  names.push_back(kIID_ISampleGrabberName);
  names.push_back(kIID_ISampleGrabberCBName);
  return names;
}

static const std::vector<const std::wstring> classIdentifierNames() {
  std::vector<const std::wstring> names;
  names.push_back(kCLSID_SampleGrabberName);
  return names;
}

QeditTypeLibraryImpl::QeditTypeLibraryImpl() :
  TypeLibrary(kTypeLibraryName,
              interfaceIdentifierNames(),
              classIdentifierNames()) {

}

IID QeditTypeLibraryImpl::IID_ISampleGrabber() const {
  return TypeLibrary::interfaceIdByName(kIID_ISampleGrabberName);
}

IID QeditTypeLibraryImpl::IID_ISampleGrabberCB() const {
  return TypeLibrary::interfaceIdByName(kIID_ISampleGrabberCBName);
}

CLSID QeditTypeLibraryImpl::CLSID_SampleGrabber() const {
  return TypeLibrary::classIdByName(kCLSID_SampleGrabberName);
}

}
