#ifndef VIDEO_CAPTURE_STREAM_FLOATING_POINT_PRECISON_H
#define VIDEO_CAPTURE_STREAM_FLOATING_POINT_PRECISON_H

#include <iostream>
#include "boost/noncopyable.hpp"

namespace VideoCapture {

class StreamFloatingPointPrecision : private boost::noncopyable {
 public:
  static const std::streamsize s_kPrecision = 2;
  StreamFloatingPointPrecision(
      std::ostream& outputStream,
      const std::streamsize& precision);
  ~StreamFloatingPointPrecision();
 private:
  std::ostream& m_outputStream;
  std::ios_base::fmtflags m_originalFormatFlags;
  std::streamsize m_originalPrecision;
};

} // VideoCapture

#endif //VIDEO_CAPTURE_STREAM_FLOATING_POINT_PRECISON_H
