#ifndef VIDEO_CAPTURE_SAMPLE_CONVERTER_H
#define VIDEO_CAPTURE_SAMPLE_CONVERTER_H

#include "boost/utility.hpp"
#include "boost/shared_ptr.hpp"

namespace VideoCapture {

template<
  typename InputSample,
  typename OutputSample> class SampleConverter : private boost::noncopyable {
 public:
  typedef SampleConverter<InputSample, OutputSample> SampleConverterType;
  typedef boost::shared_ptr<SampleConverterType> SampleConverterSharedPtr;
  typedef typename InputSample::SampleFormatType InputSampleFormatType;
  typedef typename OutputSample::SampleFormatType OutputSampleFormatType;
  typedef InputSample& InputSampleRef;
  typedef OutputSample& OutputSampleRef;

  SampleConverter() {

  }

  virtual ~SampleConverter() {

  }

  virtual OutputSampleFormatType convertedSampleFormat(
      InputSampleFormatType inputSampleFormat) const {
    OutputSampleFormatType outputSampleFormat;
    return outputSampleFormat;
  }

  virtual void convertSample(
      InputSampleRef inputSample,
      OutputSampleRef outputSample) const {

  }

  void operator()(
      InputSampleRef inputSample,
      OutputSampleRef outputSample) const {
    convertSample(inputSample, outputSample);
  }

};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_CONVERTER_H
