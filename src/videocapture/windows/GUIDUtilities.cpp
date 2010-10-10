#include "GUIDUtilities.hpp"

#include "boost/cstdint.hpp"

namespace VideoCapture {

static const std::size_t kCountElementsData4GUID = 8;
static const std::size_t kCountGroupsData4GUID = 2;
static const std::size_t kSizeGroupsData4GUID[] = {
  2,
  6 };
static const boost::uint64_t kBitsPerByte = 8;

const GUID emptyGUID() {
  GUID guid;
  guid.Data1 = 0;
  guid.Data2 = 0;
  guid.Data3 = 0;
  for (std::size_t i = 0; i < kCountElementsData4GUID; ++i) {
    guid.Data4[i] = 0;
  }
  return guid;
}

bool isEmptyGUID(const GUID& guid) {
  bool isEmpty = true;
  if (guid.Data1) {
    return !isEmpty;
  }
  if (guid.Data2) {
    return !isEmpty;
  }
  if (guid.Data3) {
    return !isEmpty;
  }
  for (std::size_t i = 0; i < kCountElementsData4GUID; ++i) {
    if (guid.Data4[i]) {
      return !isEmpty;
    }
  }
  return isEmpty;
}

std::wostream& operator<<(std::wostream& outputStream, const GUID& guid) {
  outputStream << std::hex;
  const char dash = '-';
  outputStream << guid.Data1 << dash;
  outputStream << guid.Data2 << dash;
  outputStream << guid.Data3;
  for (std::size_t group = 0, element = 0;
       group < kCountGroupsData4GUID && element < kCountElementsData4GUID;
       ++group) {
    outputStream << dash;
    std::size_t sizeGroup = kSizeGroupsData4GUID[group];
    boost::uint64_t value = 0;
    for (size_t groupElement = 0;
         groupElement < sizeGroup && element < kCountElementsData4GUID;
         ++groupElement, ++element) {
      boost::uint64_t groupElementValue = guid.Data4[element];
      // The bytes must be traversed in MSB first order
      boost::uint64_t shift = (((sizeGroup - 1) - groupElement) * kBitsPerByte);
      value |= (groupElementValue << shift);
    }
    outputStream << value;
  }
  return outputStream;
}

} // VideoCapture
