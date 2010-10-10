#ifndef VIDEO_CAPTURE_SDL_SURFACE_H
#define VIDEO_CAPTURE_SDL_SURFACE_H

#include "boost/utility.hpp"
#include "boost/shared_ptr.hpp"
#include "SDL.h"
#include "RGBVideoFormat.hpp"
#include "RGBVideoFrame.hpp"
#include "Sample.hpp"
#include "SDLSurfaceLock.hpp"

namespace VideoCapture {

class SDLSurface : private boost::noncopyable {
 public:
  typedef std::size_t SizeType;
  typedef boost::shared_ptr<SDLSurface> SDLSurfaceSharedPtr;
  typedef Sample<RGBVideoFrame> RGBVideoSample;
  typedef boost::shared_ptr<RGBVideoFrame> RGBVideoFrameSharedPtr;
  typedef RGBVideoFrame::ImageType ImageType;
  typedef RGBVideoFrame::ImageViewType ImageViewType;
  static SDLSurfaceSharedPtr createRGBSurfaceSharedPtr(
      const RGBVideoFormat& videoFormat);
  SDLSurface(SDL_Surface* pSurface);
  ~SDLSurface();
  SDL_Surface* rawSurfacePtr();
  bool renderToSurface(RGBVideoSample& videoSample);
  IntegerSize sizePixels() const;
  SizeType bitsPerPixel() const;
  SizeType sizeBytes() const;
  SizeType sizeRowBytes() const;
  RGBFormat rgbFormat() const;
  bool isInitialized() const;
  bool hasData() const;
  operator bool() const {
    return isInitialized();
  }
 private:
  template<
   typename RGBImageViewType,
   typename RGBPixelPtrType> void renderImageToSurface(
      ImageViewType& imageView);
  void initialize();
  RGBFormat rgbFormatFromPixelFormat();
  SDL_Surface* m_pSurface;
  IntegerSize m_sizePixels;
  SizeType m_bitsPerPixel;
  SizeType m_sizeBytes;
  SizeType m_sizeRowBytes;
  RGBFormat m_rgbFormat;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SDL_SURFACE_H
