#ifndef VIDEO_CAPTURE_PASS_THROUGH_SAMPLE_CONVERTER_H
#define VIDEO_CAPTURE_PASS_THROUGH_SAMPLE_CONVERTER_H

#include "SampleConverter.hpp"

namespace VideoCapture {

template<
  typename Sample> class PassThroughSampleConverter : public SampleConverter<
  Sample, Sample> {
 public:
  typedef Sample SampleType;
  typedef SampleConverter<SampleType, SampleType> SampleConverterType;
  typedef typename SampleConverterType::SampleConverterSharedPtr
  SampleConverterSharedPtr;
  typedef typename SampleType::SampleFormatType SampleFormatType;
  typedef typename SampleType& SampleRef;

  PassThroughSampleConverter()
      : SampleConverterType() {

  }

  SampleFormatType convertedSampleFormat(SampleFormatType inputSampleFormat) {
    return inputSampleFormat;
  }

  void convertSample(SampleRef inputSample, SampleRef outputSample) {
    swap(inputSample, outputSample);
  }
};

} // VideoCapture

#endif // VIDEO_CAPTURE_PASS_THROUGH_SAMPLE_CONVERTER_H
