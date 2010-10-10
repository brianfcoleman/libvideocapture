#ifndef VIDEO_CAPTURE_ROTATE_BY_180_DEGREES_SAMPLE_CONVERTER
#define VIDEO_CAPTURE_ROTATE_BY_180_DEGREES_SAMPLE_CONVERTER

#include "SampleConverter.hpp"
#include "Sample.hpp"
#include "RGBVideoFrame.hpp"
#include "RGBVideoFormat.hpp"

namespace VideoCapture {

class RotateBy180DegreesSampleConverter : public SampleConverter<
  Sample<RGBVideoFrame>, Sample<RGBVideoFrame>> {
public:
  typedef Sample<RGBVideoFrame> RGBVideoSample;
  typedef RGBVideoSample& RGBVideoSampleRef;
  RotateBy180DegreesSampleConverter();
  RGBVideoFormat convertedSampleFormat(RGBVideoFormat inputSampleFormat) const;
  void convertSample(
      RGBVideoSampleRef inputSample,
      RGBVideoSampleRef outputSample) const;
};

} // VideoCapture

#endif
