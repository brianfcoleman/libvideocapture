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

bool ByteBuffer::hasData() const {
  if (!m_pFirstBufferByte) {
    return false;
  }
  BytePtr byteArray = m_pFirstBufferByte;
  for (SizeType i = 0; i < m_sizeBytes; ++i) {
    Byte byteValue = byteArray[i];
    if (byteValue) {
      return true;
    }
  }
  return false;
}

} // VideoCapture
