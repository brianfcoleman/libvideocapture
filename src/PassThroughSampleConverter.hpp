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
  typedef typename SampleType::SampleDataSharedPtr SampleDataSharedPtr;
  typedef typename SampleType& SampleRef;

  PassThroughSampleConverter()
      : SampleConverterType() {
#ifdef DEBUG
    std::cout << "PassThroughSampleConverter::PassThroughSampleConverter";
    std::cout << std::endl;
#endif
  }

  SampleFormatType convertedSampleFormat(
      SampleFormatType inputSampleFormat) const {
#ifdef DEBUG
    std::cout << "PassThroughSampleConverter::convertedSampleFormat";
    std::cout << std::endl;
#endif
    return inputSampleFormat;
  }

  void convertSample(SampleRef inputSample, SampleRef outputSample) const {
#ifdef DEBUG
    std::cout << "PassThroughSampleConverter::convertSample";
    std::cout << std::endl;
#endif
    if (!inputSample) {
      return;
    }
    if (!outputSample) {
      return;
    }
    std::swap(inputSample, outputSample);
  }
};

} // VideoCapture

#endif // VIDEO_CAPTURE_PASS_THROUGH_SAMPLE_CONVERTER_H
