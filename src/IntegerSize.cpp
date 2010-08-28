#include "IntegerSize.h"

namespace VideoCapture {

IntegerSize::IntegerSize()
    : m_width(0),
      m_height(0) {

}

IntegerSize::IntegerSize(
    const boost::int32_t& width,
    const boost::int32_t& height)
    : m_width(width),
      m_height(height) {

}

boost::int32_t IntegerSize::width() const {
  return m_width;
}

boost::int32_t IntegerSize::height() const {
  return m_height;
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
