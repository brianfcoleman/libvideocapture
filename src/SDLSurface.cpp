#include "SDLSurface.hpp"
#include "boost/cstdint.hpp"
#include "ImageUtilities.hpp"
#ifdef DEBUG
#include <iostream>
#endif

namespace VideoCapture {

SDLSurface::SDLSurface(SDL_Surface* pSurface)
    : m_pSurface(pSurface),
      m_bitsPerPixel(0),
      m_sizeBytes(0),
      m_sizeRowBytes(0),
      m_rgbFormat(RGBNone) {
  initialize();
}

SDLSurface::~SDLSurface() {
  if (!m_pSurface) {
    return;
  }
  SDL_FreeSurface(m_pSurface);
}

SDL_Surface* SDLSurface::rawSurfacePtr() {
  if (!isInitialized()) {
    return 0;
  }
  return m_pSurface;
}

bool SDLSurface::renderToSurface(RGBVideoSample& videoSample) {
  if (!videoSample) {
    return false;
  }
  if (!isInitialized()) {
    return false;
  }
#ifdef DEBUG
  std::cout << "SDLSurface::renderToSurface" << std::endl;
#endif
  SDLSurfaceLock surfaceLock(m_pSurface);
  if (!surfaceLock) {
    return false;
  }
  RGBVideoFormat videoFormat(videoSample.sampleFormat());
  if (videoFormat.sizePixels() != sizePixels()) {
    return false;
  }
  if (videoFormat.bitsPerPixel() != bitsPerPixel()) {
    return false;
  }
  if (videoFormat.sizeRowBytes() != sizeRowBytes()) {
    return false;
  }
  if (videoFormat.sizeBytes() != sizeBytes()) {
    return false;
  }
  if (videoFormat.rgbFormat() != rgbFormat()) {
    return false;
  }
  RGBVideoFrameSharedPtr pVideoFrame(videoSample.sampleData());
  ImageViewType imageView(pVideoFrame->imageView());
  switch (rgbFormat()) {
    case RGB888:
      renderImageToSurface<rgb8_view_t, rgb8_ptr_t>(imageView);
      break;
    case RGBA8888:
      renderImageToSurface<rgba8_view_t, rgba8_ptr_t>(imageView);
      break;
    case BGR888:
      renderImageToSurface<bgr8_view_t, bgr8_ptr_t>(imageView);
      break;
    case ABGR8888:
      renderImageToSurface<abgr8_view_t, abgr8_ptr_t>(imageView);
      break;
    default:
      return false;
  }
  return true;
}

template<
  typename RGBImageViewType,
  typename RGBPixelPtrType> void SDLSurface::renderImageToSurface(
    SDLSurface::ImageViewType& imageView) {
  RGBPixelPtrType pFirstPixelSurface(
      reinterpret_cast<RGBPixelPtrType>(m_pSurface->pixels));
  RGBImageViewType imageViewSurface(boost::gil::interleaved_view(
      sizePixels().width(),
      sizePixels().height(),
      pFirstPixelSurface,
      sizeRowBytes()));
  boost::gil::copy_pixels(imageView, imageViewSurface);
#ifdef DEBUG
  bool imageViewSurfaceHasData = imageHasData(imageViewSurface);
  std::cout << "image view surface " << (imageViewSurfaceHasData ? "" : "not ");
  std::cout << "has data" << std::endl;
  std::cout << "surface " << (hasData() ? "" : "not ") << "has data";
  std::cout << std::endl;
#endif
}

void SDLSurface::initialize() {
  if (!m_pSurface) {
    return;
  }
  IntegerSize sizeSurfacePixels(m_pSurface->w, m_pSurface->h);
  m_sizePixels = sizeSurfacePixels;
  m_sizeRowBytes = m_pSurface->pitch;
  SDL_PixelFormat* pPixelFormat = m_pSurface->format;
  m_bitsPerPixel = pPixelFormat->BitsPerPixel;
  m_sizeBytes = m_sizePixels.height() * m_sizeRowBytes;
  m_rgbFormat = rgbFormatFromPixelFormat();
}

RGBFormat SDLSurface::rgbFormatFromPixelFormat() {
  SDL_PixelFormat* pPixelFormat = m_pSurface->format;
  if (m_bitsPerPixel != 24 && m_bitsPerPixel != 32) {
    return RGBNone;
  }
#ifdef DEBUG
  std::cout << "SDLSurface::rgbFormatFromPixelFormat";
  std::cout << " bitsPerPixel: " << m_bitsPerPixel;
  std::cout << " rMask: " << pPixelFormat->Rmask;
  std::cout << " gMask: " << pPixelFormat->Gmask;
  std::cout << " bMask: " << pPixelFormat->Bmask;
  std::cout << " aMask: " << pPixelFormat->Amask;
  std::cout << std::endl;
#endif
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  if (m_bitsPerPixel == 24) {
    if (pPixelFormat->Rmask == 0xff000000 &&
        pPixelFormat->Gmask == 0x00ff0000 &&
        pPixelFormat->Bmask == 0x0000ff00) {
      return RGB888;
    }
    if (pPixelFormat->Rmask == 0x0000ff00 &&
        pPixelFormat->Gmask == 0x00ff0000 &&
        pPixelFormat->Bmask == 0xff000000) {
      return BGR888;
    }
  }
  if (m_bitsPerPixel == 32) {
    if (pPixelFormat->Rmask == 0xff000000 &&
        pPixelFormat->Gmask == 0x00ff0000 &&
        pPixelFormat->Bmask == 0x0000ff00 &&
        pPixelFormat->Amask == 0x000000ff) {
      return RGBA8888;
    }
    if (pPixelFormat->Rmask == 0x000000ff &&
        pPixelFormat->GMask == 0x0000ff00 &&
        pPixelFormat->Bmask == 0x00ff0000 &&
        pPixelFormat->Amask == 0xff000000) {
      return ABGR8888;
    }
  }
  return RGBNone;
#else
  if (m_bitsPerPixel == 24) {
    if (pPixelFormat->Rmask == 0x000000ff &&
        pPixelFormat->Gmask == 0x0000ff00 &&
        pPixelFormat->Bmask == 0x00ff0000) {
      return RGB888;
    }
    if (pPixelFormat->Rmask == 0x00ff0000 &&
        pPixelFormat->Gmask == 0x0000ff00 &&
        pPixelFormat->Bmask == 0x000000ff) {
      return BGR888;
    }
  }
  if (m_bitsPerPixel == 32) {
    if (pPixelFormat->Rmask == 0x000000ff &&
        pPixelFormat->Gmask == 0x0000ff00 &&
        pPixelFormat->Bmask == 0x00ff0000 &&
        pPixelFormat->Amask == 0xff000000) {
      return RGBA8888;
    }
    if (pPixelFormat->Rmask == 0xff000000 &&
        pPixelFormat->Gmask == 0x00ff0000 &&
        pPixelFormat->Bmask == 0x0000ff00 &&
        pPixelFormat->Amask == 0x000000ff) {
      return ABGR8888;
    }
  }
  return RGBNone;
#endif
}

SDLSurface::SDLSurfaceSharedPtr SDLSurface::createRGBSurfaceSharedPtr(
    const RGBVideoFormat& videoFormat) {
  if (!videoFormat) {
    SDLSurfaceSharedPtr pSurface;
    return pSurface;
  }
  Uint32 flags = SDL_SWSURFACE;
  IntegerSize sizePixels(videoFormat.sizePixels());
  SizeType width = sizePixels.width();
  if (!width) {
    SDLSurfaceSharedPtr pSurface;
    return pSurface;
  }
  SizeType height = sizePixels.height();
  if (!height) {
    SDLSurfaceSharedPtr pSurface;
    return pSurface;
  }
  SizeType depth = videoFormat.bitsPerPixel();
  RGBFormat rgbFormat = videoFormat.rgbFormat();
  Uint32 rMask = 0;
  Uint32 gMask = 0;
  Uint32 bMask = 0;
  Uint32 aMask = 0;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  switch (rgbFormat) {
    case RGB888:
      rMask = 0xff000000;
      gMask = 0x00ff0000;
      bMask = 0x0000ff00;
      break;
    case BGR888:
      rMask = 0x0000ff00;
      gMask = 0x00ff0000;
      bMask = 0xff000000;
      break;
    case RGBA8888:
      rMask = 0xff000000;
      gMask = 0x00ff0000;
      bMask = 0x0000ff00;
      aMask = 0x000000ff;
      break;
    case ABGR8888:
      rMask = 0x000000ff;
      gMask = 0x0000ff00;
      bMask = 0x00ff0000;
      aMask = 0xff000000;
      break;
  }
#else
  switch (rgbFormat) {
    case RGB888:
      rMask = 0x000000ff;
      gMask = 0x0000ff00;
      bMask = 0x00ff0000;
      break;
    case BGR888:
      rMask = 0x00ff0000;
      gMask = 0x0000ff00;
      bMask = 0x000000ff;
      break;
    case RGBA8888:
      rMask = 0x000000ff;
      gMask = 0x0000ff00;
      bMask = 0x00ff0000;
      aMask = 0xff000000;
      break;
    case ABGR8888:
      rMask = 0xff000000;
      gMask = 0x00ff0000;
      bMask = 0x0000ff00;
      aMask = 0x000000ff;
      break;
  }
#endif
  if (!rMask && !gMask && !bMask) {
    SDLSurfaceSharedPtr pSurface;
    return pSurface;
  }
  SDLSurfaceSharedPtr pSurface(
      new SDLSurface(SDL_CreateRGBSurface(
          flags,
          width,
          height,
          depth,
          rMask,
          gMask,
          bMask,
          aMask)));
  return pSurface;
}

bool SDLSurface::isInitialized() const {
  if (!m_pSurface) {
    return false;
  }
  if (!isValidRGBFormat(rgbFormat())) {
    return false;
  }
  IntegerSize sizePixelsSurface(sizePixels());
  if (!sizePixelsSurface.width()) {
    return false;
  }
  if (!sizePixelsSurface.height()) {
    return false;
  }
  return true;
}

IntegerSize SDLSurface::sizePixels() const {
  return m_sizePixels;
}

SDLSurface::SizeType SDLSurface::bitsPerPixel() const {
  return m_bitsPerPixel;
}

SDLSurface::SizeType SDLSurface::sizeBytes() const {
  return m_sizeBytes;
}

SDLSurface::SizeType SDLSurface::sizeRowBytes() const {
  return m_sizeRowBytes;
}

RGBFormat SDLSurface::rgbFormat() const {
#ifdef DEBUG
  std::cout << "SDLSurface::rgbFormat " << m_rgbFormat << std::endl;
#endif
  return m_rgbFormat;
}

bool SDLSurface::hasData() const {
  typedef boost::uint8_t Byte;
  typedef Byte* BytePtr;
  if (!isInitialized()) {
    return false;
  }
  SizeType countBytes = sizeBytes();
  BytePtr byteArray = reinterpret_cast<BytePtr>(m_pSurface->pixels);
  for (SizeType i = 0; i < countBytes; ++i) {
    Byte byteValue = byteArray[i];
    if (byteValue) {
      return true;
    }
  }
  return false;
}

} // VideoCapture
