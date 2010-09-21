#ifndef VIDEO_CAPTURE_COM_INITIALIZER_H
#define VIDEO_CAPTURE_COM_INITIALIZER_H

#include "boost/utility.hpp"

namespace VideoCapture {

  class COMInitializer : private boost::noncopyable {
   public:
    COMInitializer();
    ~COMInitializer();
    bool isInitialized() const;
    operator bool() const {
      return isInitialized();
    }
   private:
    bool initialize();
    void uninitialize();
    bool m_isInitialized;
  };

} // VideoCapture

#endif // VIDEO_CAPTURE_COM_INITIALIZER_H
