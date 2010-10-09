#include "IntegerSize.hpp"
#include "StreamFloatingPointPrecision.hpp"

namespace VideoCapture {

IntegerSize::IntegerSize()
    : m_width(0),
      m_height(0) {

}

IntegerSize::IntegerSize(
    const std::size_t& width,
    const std::size_t& height)
    : m_width(width),
      m_height(height) {

}

std::size_t IntegerSize::width() const {
  return m_width;
}

std::size_t IntegerSize::height() const {
  return m_height;
}

bool operator==(const IntegerSize& lhs, const IntegerSize& rhs) {
  if (lhs.width() != rhs.width()) {
    return false;
  }
  if (lhs.height() != rhs.height()) {
    return false;
  }
  return true;
}

bool operator!=(const IntegerSize& lhs, const IntegerSize& rhs) {
  if (lhs.width() == rhs.width()) {
    return false;
  }
  if (lhs.height() == rhs.height()) {
    return false;
  }
  return true;
}

std::ostream& operator<<(
    std::ostream& outputStream,
    const IntegerSize& integerSize) {
  StreamFloatingPointPrecision floatingPointPrecision(
      outputStream,
      StreamFloatingPointPrecision::s_kPrecision);
  outputStream << "IntegerSize" << std::endl;
  outputStream << "width: " << integerSize.width() << std::endl;
  outputStream << "height: " << integerSize.height() << std::endl;
  return outputStream;
}

} // VideoCapture
