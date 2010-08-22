#include "DirectShow.h"

#include <iostream>
#include "TypeLibrary.h"

namespace VideoCapture {

static void DeleteMediaType(AM_MEDIA_TYPE* pMediaType);

static void FreeMediaType(AM_MEDIA_TYPE& mediaType);

boost::shared_ptr<AM_MEDIA_TYPE>
mediaTypeSharedPtr(AM_MEDIA_TYPE* pMediaType) {
  boost::shared_ptr<AM_MEDIA_TYPE> mediaTypeSharedPtr(pMediaType, 
                                                      DeleteMediaType);
  return mediaTypeSharedPtr;
}

static void DeleteMediaType(AM_MEDIA_TYPE* pMediaType) {
  if (!pMediaType) {
    return;
  }
  FreeMediaType(*(pMediaType));
  CoTaskMemFree(reinterpret_cast<void*>(pMediaType));
}

static void FreeMediaType(AM_MEDIA_TYPE& mediaType) {
  if (mediaType.cbFormat) {
      CoTaskMemFree(reinterpret_cast<void*>(mediaType.pbFormat));
      mediaType.cbFormat = 0;
      mediaType.pbFormat = 0;
  }
  if (mediaType.pUnk) {
      mediaType.pUnk->Release();
      mediaType.pUnk = 0;
  }
}

} // VideoCapture
