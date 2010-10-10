#ifndef VIDEO_CAPTURE_COM_UTILITIES_H
#define VIDEO_CAPTURE_COM_UTILITIES_H

#define BOOST_BIND_ENABLE_STDCALL
#define BOOST_MEM_FN_ENABLE_STDCALL

#include <vector>
#include "boost/shared_ptr.hpp"
#include "boost/bind.hpp"
#include "boost/mem_fn.hpp"
#include "wtypes.h"
#include "oleauto.h"

namespace VideoCapture {

boost::shared_ptr<void> comInitializerSharedPtr();

template<typename COMInterface> boost::shared_ptr<
  COMInterface> comInterfaceSharedPtr(COMInterface* pCOMInterface) {
  boost::shared_ptr<COMInterface> comInterfacePtr(
      pCOMInterface,
      boost::mem_fn(&COMInterface::Release));
  return comInterfacePtr;
}

template<typename COMInterface> boost::shared_ptr<
  COMInterface> createInstanceCOMInterface(
      const CLSID& classId,
      const IID& interfaceId) {
  COMInterface* pCOMInterface = 0;
  HRESULT result = CoCreateInstance(
      classId,
      0,
      CLSCTX_INPROC_SERVER,
      interfaceId,
      reinterpret_cast<void**>(&pCOMInterface));
  if (FAILED(result)) {
    boost::shared_ptr<COMInterface> comInterfacePtr;
    return comInterfacePtr;
  }
  boost::shared_ptr<COMInterface> comInterfacePtr(
      comInterfaceSharedPtr(pCOMInterface));
  return comInterfacePtr;
}

template<
  typename ParentCOMInterface,
  typename ChildCOMInterface> boost::shared_ptr<
  ChildCOMInterface> queryCOMInterface(
      boost::shared_ptr<ParentCOMInterface> pParentCOMInterface,
      const IID& childInterfaceId) {
  if (!pParentCOMInterface) {
    boost::shared_ptr<ChildCOMInterface> comInterfacePtr;
    return comInterfacePtr;
  }
  ChildCOMInterface* pChildCOMInterface = 0;
  HRESULT result = pParentCOMInterface->QueryInterface(
      childInterfaceId,
      reinterpret_cast<void**>(&pChildCOMInterface));
  if (FAILED(result)) {
    boost::shared_ptr<ChildCOMInterface> comInterfacePtr;
    return comInterfacePtr;
  }
  boost::shared_ptr<ChildCOMInterface> comInterfacePtr(
      comInterfaceSharedPtr(pChildCOMInterface));
  return comInterfacePtr;
}

template<typename COMInterface> std::vector<
  boost::shared_ptr<COMInterface>> comInterfaceSharedPtrVector(
      COMInterface** comInterfacePtrArray,
      const std::size_t arrayElementCount) {
  if (!comInterfacePtrArray) {
    std::vector<boost::shared_ptr<COMInterface>> comInterfacePtrVector;
    return comInterfacePtrVector;
  }
  std::vector<boost::shared_ptr<COMInterface>> comInterfacePtrVector;
  for (std::size_t i = 0; i < arrayElementCount; ++i) {
    COMInterface* pCOMInterface = comInterfacePtrArray[i];
    if (!pCOMInterface) {
      continue;
    }
    boost::shared_ptr<COMInterface> comInterfacePtr(
        comInterfaceSharedPtr(pCOMInterface));
    if (!comInterfacePtr) {
      continue;
    }
    comInterfacePtrVector.push_back(comInterfacePtr);
  }
  return comInterfacePtrVector;
}

boost::shared_ptr<VARIANT> variantSharedPtr(VARIANT* pVariant);

const std::string utf8StringFromBasicString(const BSTR basicString);

}

#endif // VIDEO_CAPTURE_COM_UTILITIES_H
