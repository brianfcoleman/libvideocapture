#ifndef VIDEO_CAPTURE_BYTE_BUFFER_H
#define VIDEO_CAPTURE_BYTE_BUFFER_H

#include "boost/cstdint.hpp"

namespace VideoCapture {

class ByteBuffer {
 public:
  typedef boost::uint8_t Byte;
  typedef Byte* BytePtr;
  typedef std::size_t SizeType;
  ByteBuffer(BytePtr pFirstBufferByte, const SizeType sizeBytes);
  BytePtr firstBufferByte() const;
  SizeType sizeBytes() const;
 private:
  BytePtr m_pFirstBufferByte;
  SizeType m_sizeBytes;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_BYTE_BUFFER_H
