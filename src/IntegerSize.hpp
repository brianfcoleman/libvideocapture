#ifndef VIDEOCAPTURE_INTEGER_SIZE_H
#define VIDEOCAPTURE_INTEGER_SIZE_H

#include "boost/cstdint.hpp"

namespace VideoCapture {

class IntegerSize {
 public:
  IntegerSize();
  IntegerSize(
      const std::size_t& width,
      const std::size_t& height);
  std::size_t width() const;
  std::size_t height() const;
 private:
  std::size_t m_width;
  std::size_t m_height;
};

bool operator==(const IntegerSize& lhs, const IntegerSize& rhs);

bool operator!=(const IntegerSize& lhs, const IntegerSize& rhs);

std::ostream& operator<<(
    std::ostream& outputStream,
    const IntegerSize& integerSize);

} // VideoCapture

#endif // VIDEOCAPTURE_INTEGER_SIZE_H
