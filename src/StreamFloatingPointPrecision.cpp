#include "StreamFloatingPointPrecision.hpp"

namespace VideoCapture {

StreamFloatingPointPrecision::StreamFloatingPointPrecision(
    std::ostream& outputStream,
    const std::streamsize& precision)
    : boost::noncopyable(),
      m_outputStream(outputStream)  {
  m_originalFormatFlags = m_outputStream.setf(
      std::ios::fixed,
      std::ios::floatfield);
  m_originalPrecision = m_outputStream.precision(precision);
}

StreamFloatingPointPrecision::~StreamFloatingPointPrecision() {
  m_outputStream.precision(m_originalPrecision);
  m_outputStream.setf(m_originalFormatFlags, std::ios::floatfield);
}

} // VideoCapture
