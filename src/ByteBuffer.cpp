#include "ByteBuffer.hpp"

namespace VideoCapture {
ByteBuffer::ByteBuffer(BytePtr pFirstBufferByte, const SizeType sizeBytes)
    : m_pFirstBufferByte(pFirstBufferByte),
      m_sizeBytes(sizeBytes) {

}

ByteBuffer::BytePtr ByteBuffer::firstBufferByte() const {
  return m_pFirstBufferByte;
}

ByteBuffer::SizeType ByteBuffer::sizeBytes() const {
  return m_sizeBytes;
}

} // VideoCapture
