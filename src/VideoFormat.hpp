#ifndef VIDEO_CAPTURE_VIDEO_FORMAT_H
#define VIDEO_CAPTURE_VIDEO_FORMAT_H

#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "IntegerSize.hpp"
#include "VideoFormatTypes.hpp"

namespace VideoCapture {

class VideoFormatImpl;

class VideoFormat {
 public:
  typedef boost::shared_ptr<VideoFormatImpl> ImplPtr;
  typedef boost::weak_ptr<VideoFormatImpl> ImplWeakPtr;
  typedef boost::uuids::uuid Uuid;
  typedef boost::uuids::nil_generator NilUuidGenerator;
  VideoFormat();
  explicit VideoFormat(const ImplPtr& pImpl);
  bool isInitialized() const;
  Uuid uuid() const;
  double framesPerSecond() const;
  IntegerSize sizePixels() const;
  boost::int32_t angleRotationDegrees() const;
  std::size_t bitsPerPixel() const;
  std::size_t sizeBytes() const;
  std::size_t sizeRowBytes() const;
  RGBFormat rgbFormat() const;
  operator bool() const {
    return isInitialized();
  }
 private:
  ImplPtr lockImplPtr() const;
  ImplWeakPtr m_pImpl;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_VIDEO_FORMAT_H
