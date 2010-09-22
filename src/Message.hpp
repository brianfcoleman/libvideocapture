#ifndef VIDEO_CAPTURE_MESSAGE_H
#define VIDEO_CAPTURE_MESSAGE_H

#include "boost/utility.hpp"
#include "boost/shared_ptr.hpp"
#include "MessageQueue.hpp"

namespace VideoCapture {

class Message : private boost::noncopyable {
 public:
  typedef Message MessageType;
  typedef boost::shared_ptr<MessageType> MessageSharedPtr;
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
