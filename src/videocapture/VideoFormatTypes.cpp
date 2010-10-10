#include "VideoFormatTypes.hpp"

namespace VideoCapture {

static const std::size_t kBitsPerByte = 8;

static std::size_t convertBitsToBytes(std::size_t countBits);

bool isValidRGBFormat(const RGBFormat rgbFormat) {
  switch (rgbFormat) {
    case RGB888:
      return true;
    case RGBA8888:
      return true;
    case BGR888:
      return true;
    case ABGR8888:
      return true;
    case RGBNone:
      return false;
    default:
      return false;
  }
}

std::size_t bitsPerPixelForRGBFormat(const RGBFormat rgbFormat) {
  switch (rgbFormat) {
    case RGB888:
      return 24;
    case RGBA8888:
      return 32;
    case BGR888:
      return 24;
    case ABGR8888:
      return 32;
    case RGBNone:
      return 0;
    default:
      return 0;
  }
}

std::size_t sizeBytesForRGBFormat(
    const IntegerSize& sizePixels,
    const RGBFormat rgbFormat) {
  if (sizePixels.width() <= 0) {
    return 0;
  }

  if (sizePixels.height() <= 0) {
    return 0;
  }

  std::size_t bitsPerPixel = bitsPerPixelForRGBFormat(rgbFormat);
  if (!bitsPerPixel) {
    return 0;
  }

  std::size_t countPixels = sizePixels.width() * sizePixels.height();
  std::size_t sizeBits = bitsPerPixel * countPixels;

  std::size_t sizeBytes = convertBitsToBytes(sizeBits);
  return sizeBytes;
}

std::size_t sizeRowBytesForRGBFormat(
    const IntegerSize& sizePixels,
    const RGBFormat rgbFormat) {
  if (sizePixels.width() <= 0) {
    return 0;
  }

  if (sizePixels.height() <= 0) {
    return 0;
  }

  std::size_t bitsPerPixel = bitsPerPixelForRGBFormat(rgbFormat);
  if (!bitsPerPixel) {
    return 0;
  }

  std::size_t sizeRowBits = bitsPerPixel * sizePixels.width();
  std::size_t sizeRowBytes = convertBitsToBytes(sizeRowBits);
  return sizeRowBytes;
}

static std::size_t convertBitsToBytes(std::size_t countBits) {
  if ((countBits % kBitsPerByte) != 0) {
    return 0;
  }

  std::size_t countBytes = countBits / kBitsPerByte;
  return countBytes;
}

} // VideoCapture
