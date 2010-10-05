#include "RGBVideoFrame.hpp"

namespace VideoCapture {

RGBVideoFrame::ImageViewType RGBVideoFrame::imageView() {
  ImageViewType imageView(boost::gil::view(m_image));
  return imageView;
}

RGBVideoFrame::ConstantImageViewType RGBVideoFrame::constImageView() const {
  ConstantImageViewType constantImageView(boost::gil::const_view(m_image));
  return constantImageView;
}

} // VideoCapture
