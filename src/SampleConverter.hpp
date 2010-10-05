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
#ifdef DEBUG
    std::cout << "SampleConverter::convertedSampleFormat" << std::endl;
#endif
    OutputSampleFormatType outputSampleFormat;
    return outputSampleFormat;
  }

  virtual void convertSample(
      InputSampleRef inputSample,
      OutputSampleRef outputSample) const {
#ifdef DEBUG
    std::cout << "SampleConverter::convertSample" << std::endl;
#endif
  }

  void operator()(
      InputSampleRef inputSample,
      OutputSampleRef outputSample) const {
    convertSample(inputSample, outputSample);
  }

};

template<
  typename InputSample,
  typename OutputSample,
  typename DerivedSampleConverter> boost::shared_ptr<
  SampleConverter<InputSample, OutputSample>> sampleConverterSharedPtr() {
  typedef SampleConverter<InputSample, OutputSample> SampleConverterType;
  typedef DerivedSampleConverter DerivedSampleConverterType;
  typedef boost::shared_ptr<SampleConverterType> SampleConverterSharedPtr;
  typedef boost::shared_ptr<
    DerivedSampleConverterType> DerivedSampleConverterSharedPtr;
  DerivedSampleConverterSharedPtr pDerivedSampleConverter(
      new DerivedSampleConverterType());
  SampleConverterSharedPtr pSampleConverter(pDerivedSampleConverter);
  return pSampleConverter;
}

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_CONVERTER_H
