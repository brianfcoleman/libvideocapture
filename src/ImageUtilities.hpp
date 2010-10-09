#ifndef VIDEO_CAPTURE_IMAGE_UTILITIES_HPP
#define VIDEO_CAPTURE_IMAGE_UTILITIES_HPP

namespace VideoCapture {

template<typename RGBImageViewType> static bool imageHasData(
    RGBImageViewType& imageView) {
  typedef RGBImageViewType::iterator IteratorType;
  typedef RGBImageViewType::value_type PixelType;
  typedef std::size_t SizeType;
  if (!imageView.is_1d_traversable()) {
    return false;
  }
  SizeType countChannels = imageView.num_channels();
  for (IteratorType iterator(imageView.begin());
       iterator != imageView.end();
       ++iterator) {
    PixelType pixel(*iterator);
    for (SizeType i = 0; i < countChannels; ++i) {
      if (pixel[i] != 0) {
        return true;
      }
    }
  }
  return false;
}

} // VideoCapture

#endif // VIDEO_CAPTURE_IMAGE_UTILITIES_HPP
