#ifndef VIDEO_CAPTURE_RGB_VIDEO_FRAME_FACTORY_H
#define VIDEO_CAPTURE_RGB_VIDEO_FRAME_FACTORY_H

#include "boost/shared_ptr.hpp"

namespace VideoCapture {

class RGBVideoFrame;
class RGBVideoFormat;

class RGBVideoFrameFactory {
 public:
  typedef RGBVideoFrame SampleDataType;
  typedef boost::shared_ptr<RGBVideoFrame> SampleDataSharedPtr;
  RGBVideoFrameFactory();
  SampleDataSharedPtr createSampleData(const RGBVideoFormat& videoFormat);
};

} // VideoCapture

#endif // VIDEO_CAPTURE_RGB_VIDEO_FRAME_FACTORY_H

