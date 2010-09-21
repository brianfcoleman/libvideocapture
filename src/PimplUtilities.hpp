#ifndef PIMPL_UTILITIES_H
#define PIMPL_UTILITIES_H

#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"

namespace VideoCapture {

template<typename ImplType> bool isInitialized(
    boost::weak_ptr<ImplType> pWeakImpl) {
  boost::shared_ptr<ImplType> pImpl(pWeakImpl.lock());
  if (!pImpl) {
    return false;
  }

  if (!pImpl->isInitialized()) {
    return false;
  }

  return true;
}

template<typename ImplType> boost::shared_ptr<ImplType> lockImplPtr(
    boost::weak_ptr<ImplType> pWeakImpl) {
  boost::shared_ptr<ImplType> pImpl(pWeakImpl.lock());
  if (!pImpl) {
    return pImpl;
  }

  if (!pImpl->isInitialized()) {
    pImpl.reset();
  }

  return pImpl;
}

} // VideoCapture
#endif // PIMPL_UTILITIES_H
