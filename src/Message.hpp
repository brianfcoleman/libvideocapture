#ifndef VIDEO_CAPTURE_MESSAGE_H
#define VIDEO_CAPTURE_MESSAGE_H

namespace VideoCapture {

class Message {
 public:
  Message() {

  }

  virtual ~Message() {

  }

  virtual void processMessage() {

  }

  void operator()() {
    processMessage();
  }
};

} // VideoCapture

#endif // VIDEO_CAPTURE_MESSAGE_H
