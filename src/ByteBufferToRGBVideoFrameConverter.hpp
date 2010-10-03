#ifndef VIDEO_CAPTURE_BYTE_BUFFER_TO_RGB_VIDEO_FRAME_CONVERTER_H
#define VIDEO_CAPTURE_BYTE_BUFFER_TO_RGB_VIDEO_FRAME_CONVERTER_H

#include "ByteBuffer.hpp"

namespace VideoCapture {

template<typename Sample> class ByteBufferToRGBVideoFrameConverter {
 public:
  typedef Sample SampleType;
  typedef SampleType& SampleRef;
  typedef typename SampleType::SampleDataType SampleDataType;
  typedef typename SampleType::SampleFormatType SampleFormatType;
  typedef typename SampleType::SampleDataSharedPtr SampleDataSharedPtr;

  ByteBufferToRGBVideoFrameConverter() {

  }

  void operator()(ByteBuffer buffer, SampleRef sample) {
    if (!sample) {
      return;
    }
    RGBVideoFormat videoFormat(sample.sampleFormat());
    if (!videoFormat) {
      return;
    }
    if (buffer.sizeBytes() != videoFormat.sizeBytes()) {
      return;
    }
    RGBFormat rgbFormat = videoFormat.rgbFormat();
    switch (rgbFormat) {
      case RGB888:
        copyBufferIntoSample<rgb8c_view_t, rgb8c_ptr_t>(
            buffer,
            sample);
      case RGBA8888:
        copyBufferIntoSample<rgba8c_view_t, rgba8c_ptr_t>(
            buffer,
            sample);
      case BGR888:
        copyBufferIntoSample<bgr8c_view_t, bgr8c_ptr_t>(
            buffer,
            sample);
      case ABGR8888:
        copyBufferIntoSample<abgr8c_view_t, abgr8c_ptr_t>(
            buffer,
            sample);
    }
  }

 private:
    template<
      typename RGBImageViewType,
      typename RGBPixelPtrType> static void copyBufferIntoSample(
          ByteBuffer buffer,
          SampleRef sample) {
      if (!sample) {
        return;
      }
      RGBVideoFormat videoFormat(sample.sampleFormat());
      IntegerSize sizePixels(videoFormat.sizePixels());
      if (sizePixels.width() <= 0) {
        return;
      }
      if (sizePixels.height() <= 0) {
        return;
      }
      std::size_t sizeRowBytes = videoFormat.sizeRowBytes();
      if (!sizeRowBytes) {
        return;
      }
      RGBPixelPtrType pFirstPixel = reinterpret_cast<RGBPixelPtrType>(
          buffer.firstBufferByte());
      RGBImageViewType imageViewBuffer = boost::gil::interleaved_view(
          sizePixels.width(),
          sizePixels.height(),
          pFirstPixel,
          sizeRowBytes);
      SampleDataSharedPtr pSampleData(
          sample.sampleData());
      RGBVideoFrame::ImageViewType imageViewSample(pSampleData->imageView());
      imageViewSample = imageViewBuffer;
    }

};

} // VideoCapture

#endif // VIDEO_CAPTURE_BYTE_BUFFER_TO_RGB_VIDEO_FRAME_CONVERTER_H
