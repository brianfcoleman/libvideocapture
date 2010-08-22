#include "ComUtilities.h"

#include "boost/scoped_array.hpp"

namespace VideoCapture {

static const std::string kEmptyString("");  
  
boost::shared_ptr<void> comInitializerSharedPtr() {
  CoInitialize(static_cast<LPVOID>(0));
  boost::shared_ptr<void> comInitializerSharedPtr(static_cast<void*>(0),
                                                  boost::bind(CoUninitialize));
  return comInitializerSharedPtr;
}

boost::shared_ptr<VARIANT> variantSharedPtr(VARIANT* pVariant) {
  if (!pVariant) {
    boost::shared_ptr<VARIANT> variantSharedPtr;
    return variantSharedPtr;
  }
  ::VariantInit(pVariant);
  boost::shared_ptr<VARIANT> variantSharedPtr(pVariant, ::VariantClear);
  return variantSharedPtr;
}

const std::string utf8StringFromBasicString(const BSTR basicString) {
  if (!basicString) {
    return kEmptyString;
  }
  UINT countCharsBasicString = SysStringLen(basicString);
  if (!countCharsBasicString) {
    return kEmptyString;
  }
  UINT codePage = CP_UTF8;
  DWORD flags = 0;
  LPCWSTR wideCharString = static_cast<LPCWSTR>(basicString);
  int countCharsWideCharString = static_cast<int>(countCharsBasicString);
  LPSTR pUTF8String = 0;
  int countBytesUTF8String = 0;
  LPCSTR pDefaultChar = 0;
  LPBOOL pUsedDefaultChar = 0;
  countBytesUTF8String = WideCharToMultiByte(codePage,
                                             flags,
                                             wideCharString,
                                             countCharsWideCharString,
                                             pUTF8String,
                                             countBytesUTF8String,
                                             pDefaultChar,
                                             pUsedDefaultChar);
  if (countBytesUTF8String <= 0) {
   return kEmptyString;
  }
  boost::scoped_array<char> charArrayUTF8String(new char[countBytesUTF8String]);
  if (!charArrayUTF8String) {
    return kEmptyString;
  }
  pUTF8String = static_cast<LPSTR>(charArrayUTF8String.get());
  WideCharToMultiByte(codePage,
                      flags,
                      wideCharString,
                      countCharsWideCharString,
                      pUTF8String,
                      countBytesUTF8String,
                      pDefaultChar,
                      pUsedDefaultChar);
  const std::string utf8String(charArrayUTF8String.get());
  return utf8String;
}
  
}