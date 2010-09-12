#ifndef VIDEO_CAPTURE_SAMPLE_QUEUE_H
#define VIDEO_CAPTURE_SAMPLE_QUEUE_H

#include "boost/utility.hpp"

namespace VideoCapture {

template<typename Sample> class SampleQueue : private boost::noncopyable {
 public:
  typedef Sample SampleType;
  SampleQueue() {

  };

  virtual ~SampleQueue() {

  }

  virtual void addSample(const SampleType& sample) = 0;

  virtual SampleType removeSample() = 0;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_SAMPLE_QUEUE_H
