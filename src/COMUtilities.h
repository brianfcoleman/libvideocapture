#ifndef COM_UTILITIES_H
#define COM_UTILITIES_H

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

template<typename COMInterface> boost::shared_ptr<COMInterface>
comInterfaceSharedPtr(COMInterface* pCOMInterface) {
  boost::shared_ptr<COMInterface> comInterfaceSharedPtr(pCOMInterface,
                                         boost::mem_fn(&COMInterface::Release));
  return comInterfaceSharedPtr;
}

template<typename COMInterface> std::vector<boost::shared_ptr<COMInterface>>
comInterfaceSharedPtrVector(
  COMInterface** comInterfacePtrArray,
  const std::size_t arrayElementCount) {
  if (!comInterfacePtrArray) {
    std::vector<boost::shared_ptr<COMInterface>> comInterfaceSharedPtrVector;
    return comInterfaceSharedPtrVector;
  }
  std::vector<boost::shared_ptr<COMInterface>> comInterfaceSharedPtrVector;
  for (std::size_t i = 0; i < arrayElementCount; ++i) {
    COMInterface* pCOMInterface = comInterfacePtrArray[i];
    if (!pCOMInterface) {
      continue;
    }
    boost::shared_ptr<COMInterface> 
    comInterfacePtr(comInterfaceSharedPtr<COMInterface>(pCOMInterface));
    if (!comInterfacePtr) {
      continue;
    }
    comInterfaceSharedPtrVector.push_back(comInterfacePtr);
  }
  return comInterfaceSharedPtrVector;
}

boost::shared_ptr<VARIANT> variantSharedPtr(VARIANT* pVariant);

const std::string utf8StringFromBasicString(const BSTR basicString);

}

#endif // COM_UTILITIES_H