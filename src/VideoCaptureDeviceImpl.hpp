#ifndef VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_IMPL_H
#define VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_IMPL_H

#include <string>
#include <map>
#include <list>
#include "boost/utility.hpp"
#include "boost/cstdint.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "DirectShow.hpp"
#include "SampleGrabberCallback.hpp"
#include "VideoFormatImpl.hpp"
#include "RGBVideoFormat.hpp"

namespace VideoCapture {

class VideoCaptureDeviceImpl : private boost::noncopyable {
 public:
  typedef boost::shared_ptr<IMoniker> IMonikerSharedPtr;
  typedef boost::shared_ptr<IBindCtx> IBindCtxSharedPtr;
  typedef boost::shared_ptr<IPropertyBag> IPropertyBagSharedPtr;
  typedef boost::shared_ptr<VARIANT> VariantSharedPtr;
  typedef boost::shared_ptr<IFilterGraph2> IFilterGraph2SharedPtr;
  typedef boost::shared_ptr<
    ICaptureGraphBuilder2> ICaptureGraphBuilder2SharedPtr;
  typedef boost::shared_ptr<IBaseFilter> IBaseFilterSharedPtr;
  typedef boost::shared_ptr<ISampleGrabber> ISampleGrabberSharedPtr;
  typedef boost::shared_ptr<
    SampleGrabberCallback> SampleGrabberCallbackSharedPtr;
  typedef SampleGrabberCallback::SampleProducerCallbackSharedPtr
  SampleProducerCallbackSharedPtr;
  typedef SampleGrabberCallback::SampleProducerCallbackSet
  SampleProducerCallbackSet;
  typedef boost::shared_ptr<IMediaControl> IMediaControlSharedPtr;
  typedef boost::shared_ptr<IAMStreamConfig> IAMStreamConfigSharedPtr;
  typedef boost::shared_ptr<IAMVideoControl> IAMVideoControlSharedPtr;
  typedef boost::shared_ptr<IPin> IPinSharedPtr;
  typedef boost::shared_ptr<VideoFormatImpl> VideoFormatSharedPtr;
  typedef boost::uuids::uuid Uuid;
  typedef boost::uuids::random_generator UuidGenerator;
  typedef boost::uuids::nil_generator NilUuidGenerator;
  typedef std::map<Uuid, VideoFormatSharedPtr> VideoFormatMap;
  typedef std::pair<Uuid, VideoFormatSharedPtr> UuidVideoFormatPair;
  typedef VideoFormatMap::const_iterator VideoFormatConstIterator;
  typedef std::list<RGBVideoFormat> RGBVideoFormatList;
  typedef RGBVideoFormatList::const_iterator RGBVideoFormatConstIterator;

  VideoCaptureDeviceImpl(
      const QeditTypeLibrary& qeditTypeLibrary,
      const IMonikerSharedPtr& pMoniker,
      UuidGenerator& uuidGenerator);
  ~VideoCaptureDeviceImpl();
  bool isInitialized() const;
  std::string name() const;
  bool addSampleProducerCallback(
      const SampleProducerCallbackSharedPtr& pSampleProducerCallback);
  bool removeSampleProducerCallback(
      const SampleProducerCallbackSharedPtr& pSampleProducerCallback);
  bool startCapturing();
  bool stopCapturing();
  double countFramesCapturedPerSecond() const;
  RGBVideoFormatList videoFormatList() const;
  RGBVideoFormat currentVideoFormat() const;
  bool setCurrentVideoFormat(const RGBVideoFormat& videoFormat);
  operator bool() const {
    return isInitialized();
  }
 private:
  static const boost::uint32_t s_kUseBufferCB = 1;
  static const std::wstring s_kFriendlyName;
  static const std::wstring s_kNameCaptureFilter;
  static const std::wstring s_kNameSampleGrabberFilter;
  static const std::wstring s_kNameNullRendererFilter;
  static const std::string s_kEmptyString;
  static const double s_kOneSecondNs;
  bool initialize(UuidGenerator& uuidGenerator);
  bool initName();
  bool initCaptureGraph();
  bool buildCaptureGraph();
  bool initCaptureGraphBuilder();
  bool initSampleGrabber();
  bool initSampleGrabberCallback();
  bool initMediaControl();
  bool initVideoControl();
  bool initCapturePin();
  bool initStreamConfig();
  bool initVideoFormatMap(UuidGenerator& uuidGenerator);
  RGBVideoFormat rgbVideoFormatFromPair(
      const UuidVideoFormatPair& uuidVideoFormatPair) const;
  bool m_isInitialized;
  std::string m_name;
  IID m_IID_ISampleGrabber;
  IID m_IID_ISampleGrabberCB;
  IMonikerSharedPtr m_pMoniker;
  IBindCtxSharedPtr m_pBindContext;
  IFilterGraph2SharedPtr m_pFilterGraph;
  ICaptureGraphBuilder2SharedPtr m_pCaptureGraphBuilder;
  IBaseFilterSharedPtr m_pCaptureFilter;
  IBaseFilterSharedPtr m_pSampleGrabberFilter;
  IBaseFilterSharedPtr m_pNullRendererFilter;
  ISampleGrabberSharedPtr m_pSampleGrabber;
  SampleProducerCallbackSet m_sampleProducerCallbackSet;
  SampleGrabberCallbackSharedPtr m_pSampleGrabberCallback;
  IMediaControlSharedPtr m_pMediaControl;
  IAMStreamConfigSharedPtr m_pStreamConfig;
  IAMVideoControlSharedPtr m_pVideoControl;
  IPinSharedPtr m_pCapturePin;
  Uuid m_currentVideoFormatUuid;
  VideoFormatMap m_videoFormatsByUuid;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_IMPL_H
