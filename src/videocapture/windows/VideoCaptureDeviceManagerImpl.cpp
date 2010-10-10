#include "VideoCaptureDeviceManagerImpl.hpp"
#include "boost/cstdint.hpp"

namespace VideoCapture {

VideoCaptureDeviceManagerImpl::VideoCaptureDeviceManagerImpl(
    const VideoCaptureDeviceManagerImpl::MessageQueueType& messageQueue)
    : m_isInitialized(false),
      m_messageQueue(messageQueue) {
  m_isInitialized = initialize();
}

bool VideoCaptureDeviceManagerImpl::initialize() {
  if (!m_comInitializer) {
    return false;
  }

  if (!m_qeditTypeLibrary) {
    return false;
  }

  if (!initVideoCaptureDeviceImplList()) {
    return false;
  }

  return true;
}

bool VideoCaptureDeviceManagerImpl::initVideoCaptureDeviceImplList() {
	ICreateDevEnumSharedPtr pSystemDeviceEnumerator(
      createInstanceCOMInterface<ICreateDevEnum>(
          CLSID_SystemDeviceEnum,
          IID_ICreateDevEnum));
	if (!pSystemDeviceEnumerator) {
		return false;
	}

  IEnumMoniker* pEnumMoniker = 0;
  boost::uint32_t flags = 0;
	HRESULT result = pSystemDeviceEnumerator->CreateClassEnumerator(
		CLSID_VideoInputDeviceCategory,
		&pEnumMoniker,
		flags);
	if (result != S_OK) {
		return false;
	}
  IEnumMonikerSharedPtr pVideoInputDeviceEnumerator(
      comInterfaceSharedPtr(pEnumMoniker));
	if (!pVideoInputDeviceEnumerator) {
		return false;
	}

	do {
		IMoniker* pMoniker;
    boost::uint32_t countElements = 1;
    boost::uint32_t countElementsFetched = 0;
		result = pVideoInputDeviceEnumerator->Next(
        countElements,
        &pMoniker,
        &countElementsFetched);
		if (result != S_OK)	{
			break;
		}
    IMonikerSharedPtr pVideoInputDeviceMoniker(
        comInterfaceSharedPtr(pMoniker));
		if (!pVideoInputDeviceMoniker) {
			continue;
		}
    VideoCaptureDeviceImplSharedPtr pVideoCaptureDevice(
        new VideoCaptureDeviceImpl(
            m_qeditTypeLibrary,
            pVideoInputDeviceMoniker,
            m_uuidGenerator));
		if (!pVideoCaptureDevice)	{
			continue;
		}
		m_videoCaptureDeviceImplList.push_back(pVideoCaptureDevice);
	}	while (result == S_OK);

  SizeType countVideoCaptureDevices = m_videoCaptureDeviceImplList.size();
  if (countVideoCaptureDevices <= 0) {
    return false;
  }

  return true;
}

bool VideoCaptureDeviceManagerImpl::isInitialized() const {
  return m_isInitialized;
}

VideoCaptureDeviceManagerImpl::VideoCaptureDeviceList
VideoCaptureDeviceManagerImpl::videoCaptureDeviceList() const {
  if (!isInitialized()) {
    VideoCaptureDeviceList emptyList;
    return emptyList;
  }

  VideoCaptureDeviceList deviceList;
  for (VideoCaptureDeviceImplConstIterator iterator(
           m_videoCaptureDeviceImplList.begin());
       iterator != m_videoCaptureDeviceImplList.end();
       ++iterator) {
    VideoCaptureDeviceImplSharedPtr pImpl(*iterator);
    VideoCaptureDevice videoCaptureDevice(pImpl, m_messageQueue);
    if (!videoCaptureDevice) {
      continue;
    }
    deviceList.push_back(videoCaptureDevice);
  }
  return deviceList;
}

VideoCaptureDeviceManagerImpl::MessageQueueType
VideoCaptureDeviceManagerImpl::messageQueue() {
  return m_messageQueue;
}

} // VideoCapture
