#include "COMInitializer.hpp"
#include "objbase.h"

namespace VideoCapture {

COMInitializer::COMInitializer()
    : m_isInitialized(false) {
  m_isInitialized = initialize();
}

COMInitializer::~COMInitializer() {
  uninitialize();
}

bool COMInitializer::initialize() {
  HRESULT result = CoInitializeEx(
      static_cast<void*>(0),
      COINIT_APARTMENTTHREADED);
  if (result == S_OK) {
    return true;
  }
  if (result == S_FALSE) {
    return true;
  }
  return false;
}

void COMInitializer::uninitialize() {
  if (!isInitialized()) {
    return;
  }
  CoUninitialize();
}

bool COMInitializer::isInitialized() const {
  return m_isInitialized;
}

} // VideoCapture
