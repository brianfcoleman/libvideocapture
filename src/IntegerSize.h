#ifndef VIDEOCAPTURE_INTEGER_SIZE_H
#define VIDEOCAPTURE_INTEGER_SIZE_H

#include "boost/cstdint.hpp"
#include "StreamFloatingPointPrecision.h"

namespace VideoCapture {

class IntegerSize {
 public:
  IntegerSize();
  IntegerSize(
      const boost::int32_t& width,
      const boost::int32_t& height);
  boost::int32_t width() const;
  boost::int32_t height() const;
 private:
  boost::int32_t m_width;
  boost::int32_t m_height;
};

std::ostream& operator<<(
    std::ostream& outputStream,
    const IntegerSize& integerSize);

} // VideoCapture

#endif // VIDEOCAPTURE_INTEGER_SIZE_H
