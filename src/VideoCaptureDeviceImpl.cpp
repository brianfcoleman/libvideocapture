#include "VideoCaptureDeviceImpl.hpp"

namespace VideoCapture {

static RGBVideoFormat firstVideoFormatFromList(
    VideoCaptureDeviceImpl::RGBVideoFormatList videoFormatList);

const std::wstring VideoCaptureDeviceImpl::s_kFriendlyName(L"FriendlyName");
const std::wstring VideoCaptureDeviceImpl::s_kNameCaptureFilter(
    L"Capture Filter");
const std::wstring VideoCaptureDeviceImpl::s_kNameSampleGrabberFilter(
    L"Sample Grabber Filter");
const std::wstring VideoCaptureDeviceImpl::s_kNameNullRendererFilter(
    L"Null Renderer Filter");
const std::string VideoCaptureDeviceImpl::s_kEmptyString("");
const double VideoCaptureDeviceImpl::s_kOneSecondNs = 1.0e9;

VideoCaptureDeviceImpl::VideoCaptureDeviceImpl(
    const QeditTypeLibrary& qeditTypeLibrary,
    const VideoCaptureDeviceImpl::IMonikerSharedPtr& pMoniker,
    VideoCaptureDeviceImpl::UuidGenerator& uuidGenerator)
    : m_isInitialized(false),
      m_name(s_kEmptyString),
      m_IID_ISampleGrabber(qeditTypeLibrary.IID_ISampleGrabber()),
      m_IID_ISampleGrabberCB(qeditTypeLibrary.IID_ISampleGrabberCB()),
      m_pMoniker(pMoniker),
      m_pFilterGraph(createInstanceCOMInterface<IFilterGraph2>(
          CLSID_FilterGraph,
          IID_IFilterGraph2)),
      m_pCaptureGraphBuilder(createInstanceCOMInterface<ICaptureGraphBuilder2>(
          CLSID_CaptureGraphBuilder2,
          IID_ICaptureGraphBuilder2)),
      m_pSampleGrabberFilter(createInstanceCOMInterface<IBaseFilter>(
          qeditTypeLibrary.CLSID_SampleGrabber(),
          IID_IBaseFilter)),
      m_pNullRendererFilter(createInstanceCOMInterface<IBaseFilter>(
          qeditTypeLibrary.CLSID_NullRenderer(),
          IID_IBaseFilter)),
      m_currentVideoFormatUuid(NilUuidGenerator()()) {
  m_isInitialized = initialize(uuidGenerator);
  RGBVideoFormat videoFormat(firstVideoFormatFromList(videoFormatList()));
  if (!videoFormat) {
    return;
  }
  m_isInitialized = setCurrentVideoFormat(videoFormat);
}

VideoCaptureDeviceImpl::~VideoCaptureDeviceImpl() {
  stopCapturing();
}

bool VideoCaptureDeviceImpl::initialize(
    VideoCaptureDeviceImpl::UuidGenerator& uuidGenerator) {
  if (!initName()) {
    return false;
  }

  if (!initCaptureGraph()) {
    return false;
  }

  if (!initVideoFormatMap(uuidGenerator)) {
    return false;
  }

  return true;
}

bool VideoCaptureDeviceImpl::initName() {
  if (!m_pMoniker) {
    return false;
  }
  IBindCtx* pBindContext = 0;
  HRESULT result = CreateBindCtx(0, &pBindContext);
  if (FAILED(result)) {
    return false;
  }
  IBindCtxSharedPtr bindContextPtr(comInterfaceSharedPtr(pBindContext));
  if (!bindContextPtr) {
    return false;
  }

  IPropertyBag* pPropertyBag = 0;
  result = m_pMoniker->BindToStorage(
      bindContextPtr.get(),
      0,
      IID_IPropertyBag,
      reinterpret_cast<void**>(&pPropertyBag));
  if (FAILED(result)) {
    return false;
  }
  IPropertyBagSharedPtr propertyBagPtr(comInterfaceSharedPtr(pPropertyBag));
  if (!propertyBagPtr) {
    return false;
  }

  VARIANT variant;
  VariantSharedPtr variantPtr(variantSharedPtr(&variant));
  result = propertyBagPtr->Read(s_kFriendlyName.c_str(), variantPtr.get(), 0);
  if (FAILED(result)) {
    return false;
  }

  const BSTR basicString = variantPtr->bstrVal;
  if (!basicString) {
    return false;
  }
  std::string friendlyName(utf8StringFromBasicString(basicString));
  m_name = friendlyName;
  return true;
}

bool VideoCaptureDeviceImpl::initCaptureGraph() {
  if(!initCaptureGraphBuilder()) {
    return false;
  }

  if (!buildCaptureGraph()) {
    return false;
  }

  if (!initSampleGrabber()) {
    return false;
  }

  SampleProducerCallbackList emptyList;
  if (!initSampleGrabberCallback(emptyList)) {
    return false;
  }

  if (!initMediaControl()) {
    return false;
  }

  if (!initStreamConfig()) {
    return false;
  }

  initVideoControl();

  return true;
}

bool VideoCaptureDeviceImpl::initCaptureGraphBuilder() {
  if (!m_pFilterGraph) {
    return false;
  }

  if (!m_pCaptureGraphBuilder) {
    return false;
  }

  HRESULT result = m_pCaptureGraphBuilder->SetFiltergraph(m_pFilterGraph.get());
  return SUCCEEDED(result);
}

bool VideoCaptureDeviceImpl::buildCaptureGraph() {
  if (!m_pFilterGraph) {
    return false;
  }

  IBindCtx* pBindContext = 0;
  HRESULT result = CreateBindCtx(0, &pBindContext);
  if (FAILED(result)) {
    return false;
  }
  IBindCtxSharedPtr bindContextPtr(comInterfaceSharedPtr(pBindContext));
  m_pBindContext = bindContextPtr;
  if (!m_pBindContext) {
    return false;
  }

  IBaseFilter* pCaptureFilter = 0;
  result = m_pFilterGraph->AddSourceFilterForMoniker(
      m_pMoniker.get(),
      m_pBindContext.get(),
      s_kNameCaptureFilter.c_str(),
      &pCaptureFilter);
  if (FAILED(result)) {
    return false;
  }
  IBaseFilterSharedPtr captureFilterPtr(comInterfaceSharedPtr(pCaptureFilter));
  m_pCaptureFilter = captureFilterPtr;
  if (!m_pCaptureFilter) {
    return false;
  }

  if (!m_pSampleGrabberFilter) {
    return false;
  }

  result = m_pFilterGraph->AddFilter(
      m_pSampleGrabberFilter.get(),
      s_kNameSampleGrabberFilter.c_str());
  if (FAILED(result)) {
    return false;
  }

  if (!m_pNullRendererFilter) {
    return false;
  }

  result = m_pFilterGraph->AddFilter(
      m_pNullRendererFilter.get(),
      s_kNameNullRendererFilter.c_str());
  if (FAILED(result)) {
    return false;
  }

  if (!m_pCaptureGraphBuilder) {
    return false;
  }

  result = m_pCaptureGraphBuilder->RenderStream(
      &PIN_CATEGORY_CAPTURE,
      &MEDIATYPE_Video,
      m_pCaptureFilter.get(),
      m_pSampleGrabberFilter.get(),
      m_pNullRendererFilter.get());
  return SUCCEEDED(result);
}

bool VideoCaptureDeviceImpl::initSampleGrabber() {
  if (!m_pSampleGrabberFilter) {
    return false;
  }

  ISampleGrabberSharedPtr sampleGrabberPtr(
      queryCOMInterface<IBaseFilter, ISampleGrabber>(
      m_pSampleGrabberFilter,
      m_IID_ISampleGrabber));
  m_pSampleGrabber = sampleGrabberPtr;
  if (!m_pSampleGrabber) {
    return false;
  }

  HRESULT result = m_pSampleGrabber->SetOneShot(FALSE);
  if (FAILED(result)) {
    return false;
  }

  result = m_pSampleGrabber->SetBufferSamples(FALSE);
  if (FAILED(result)) {
    return false;
  }

  return true;
}

bool VideoCaptureDeviceImpl::initSampleGrabberCallback(
    const VideoCaptureDeviceImpl::SampleProducerCallbackList&
    sampleProducerCallbackList) {
  if (!m_pSampleGrabber) {
    return false;
  }

  HRESULT result = m_pSampleGrabber->SetCallback(0, s_kUseBufferCB);
  if (FAILED(result)) {
    return false;
  }

  SampleGrabberCallbackSharedPtr sampleGrabberCallbackPtr(
      new SampleGrabberCallback(
          m_IID_ISampleGrabberCB,
          sampleProducerCallbackList));
  m_pSampleGrabberCallback = sampleGrabberCallbackPtr;
  if (!m_pSampleGrabberCallback) {
    return false;
  }

  result = m_pSampleGrabber->SetCallback(
      m_pSampleGrabberCallback.get(), s_kUseBufferCB);
  return SUCCEEDED(result);
}

bool VideoCaptureDeviceImpl::initMediaControl() {
  if (!m_pFilterGraph) {
    return false;
  }

  IMediaControlSharedPtr mediaControlPtr(
      queryCOMInterface<IFilterGraph2, IMediaControl>(
          m_pFilterGraph,
          IID_IMediaControl));
  m_pMediaControl = mediaControlPtr;
  if (!m_pMediaControl) {
    return false;
  }

  return true;
}

bool VideoCaptureDeviceImpl::initVideoControl()
{
  if (!m_pCaptureGraphBuilder) {
    return false;
  }

  if (!m_pCaptureFilter) {
    return false;
  }

  IAMVideoControl* pVideoControl = 0;
  HRESULT result = m_pCaptureGraphBuilder->FindInterface(
      &PIN_CATEGORY_CAPTURE,
      0,
      m_pCaptureFilter.get(),
      IID_IAMVideoControl,
      reinterpret_cast<void**>(&pVideoControl));
  if (FAILED(result)) {
    return false;
  }
  IAMVideoControlSharedPtr videoControlPtr(comInterfaceSharedPtr(pVideoControl));
  m_pVideoControl = videoControlPtr;
  if (!m_pVideoControl) {
    return false;
  }

  return true;
}

bool VideoCaptureDeviceImpl::initCapturePin()
{
  if (!m_pCaptureGraphBuilder) {
    return false;
  }

  IPin* pCapturePin = 0;
  HRESULT result = m_pCaptureGraphBuilder->FindPin(
      m_pCaptureFilter.get(),
      PINDIR_OUTPUT,
      &PIN_CATEGORY_CAPTURE,
      &MEDIATYPE_Video,
      FALSE,
      0,
      &pCapturePin);
  if (FAILED(result)) {
    return false;
  }
  IPinSharedPtr capturePinPtr(comInterfaceSharedPtr(pCapturePin));
  m_pCapturePin = capturePinPtr;
  if (!m_pCapturePin) {
    return false;
  }

  return true;
}

bool VideoCaptureDeviceImpl::initStreamConfig() {
  if (!m_pCaptureGraphBuilder) {
    return false;
  }

  if (!m_pCaptureFilter) {
    return false;
  }

  IAMStreamConfig* pStreamConfig = 0;
  HRESULT result = m_pCaptureGraphBuilder->FindInterface(
      &PIN_CATEGORY_CAPTURE,
      0,
      m_pCaptureFilter.get(),
      IID_IAMStreamConfig,
      reinterpret_cast<void**>(&pStreamConfig));
  if (FAILED(result)) {
    return false;
  }
  IAMStreamConfigSharedPtr streamConfigPtr(comInterfaceSharedPtr(pStreamConfig));
  m_pStreamConfig = streamConfigPtr;
  if (!m_pStreamConfig) {
    return false;
  }

  return true;
}

bool VideoCaptureDeviceImpl::initVideoFormatMap(
    VideoCaptureDeviceImpl::UuidGenerator& uuidGenerator) {
  if (!m_pStreamConfig) {
    return false;
  }

  int countCapabilities = 0;
  int sizeBytes = 0;
  HRESULT result = m_pStreamConfig->GetNumberOfCapabilities(
      &countCapabilities,
      &sizeBytes);
  if (FAILED(result)) {
    return false;
  }

  if (sizeBytes != sizeof(VIDEO_STREAM_CONFIG_CAPS)) {
    return false;
  }

  for (int index = 0; index < countCapabilities; ++index) {
    Uuid uuid(uuidGenerator());
    VideoFormatSharedPtr pVideoFormat(
        new VideoFormatImpl(m_pStreamConfig, index, uuid));
    if (!pVideoFormat) {
      continue;
    }
    if (!pVideoFormat->isRGBFormat()) {
      continue;
    }

    m_videoFormatsByUuid[pVideoFormat->uuid()] = pVideoFormat;
  }

  return (!m_videoFormatsByUuid.empty());
}

bool VideoCaptureDeviceImpl::isInitialized() const {
  return m_isInitialized;
}

std::string VideoCaptureDeviceImpl::name() const {
  return m_name;
}

bool VideoCaptureDeviceImpl::startCapturing(
      const SampleProducerCallbackList& sampleProducerCallbackList) {
  if (!isInitialized()) {
    return false;
  }

  if (!stopCapturing()) {
    return false;
  }

  if (!initSampleGrabberCallback(sampleProducerCallbackList)) {
    return false;
  }

  HRESULT result = m_pMediaControl->Run();
  return SUCCEEDED(result);
}

bool VideoCaptureDeviceImpl::stopCapturing() {
  if (!isInitialized()) {
    return true;
  }

  HRESULT result;
  result = m_pMediaControl->Stop();
  return SUCCEEDED(result);
}

double VideoCaptureDeviceImpl::countFramesCapturedPerSecond() const {
  if (!isInitialized()) {
    return 0;
  }

  if (!m_pVideoControl) {
    return 0;
  }

  if (!m_pCapturePin) {
    return 0;
  }

  boost::int64_t framePeriod = 0;
  HRESULT result = m_pVideoControl->GetCurrentActualFrameRate(
      m_pCapturePin.get(),
      &framePeriod);
  if (FAILED(result)) {
    return 0;
  }
  if (framePeriod <= 0) {
    return 0;
  }
  double framePeriodNs = framePeriod * 100.0;
  double frameRate = s_kOneSecondNs / framePeriodNs;
  return frameRate;
}

VideoCaptureDeviceImpl::RGBVideoFormatList
VideoCaptureDeviceImpl::videoFormatList() const {
  if (!isInitialized()) {
    RGBVideoFormatList emptyList;
    return emptyList;
  }

  RGBVideoFormatList rgbVideoFormatList;
  for (VideoFormatConstIterator iterator(m_videoFormatsByUuid.begin());
       iterator != m_videoFormatsByUuid.end();
       ++iterator) {
    const UuidVideoFormatPair uuidVideoFormatPair(*iterator);
    RGBVideoFormat rgbVideoFormat(rgbVideoFormatFromPair(uuidVideoFormatPair));
    if (!rgbVideoFormat) {
      continue;
    }
    rgbVideoFormatList.push_back(rgbVideoFormat);
  }
  return rgbVideoFormatList;
}

RGBVideoFormat VideoCaptureDeviceImpl::rgbVideoFormatFromPair(
    const UuidVideoFormatPair& uuidVideoFormatPair) const {
  const VideoFormatSharedPtr pVideoFormat(uuidVideoFormatPair.second);
  if (!pVideoFormat) {
    RGBVideoFormat rgbVideoFormat;
    return rgbVideoFormat;
  }
  RGBVideoFormat rgbVideoFormat(pVideoFormat);
  return rgbVideoFormat;
}

RGBVideoFormat VideoCaptureDeviceImpl::currentVideoFormat() const {
  if (!isInitialized()) {
    RGBVideoFormat rgbVideoFormat;
    return rgbVideoFormat;
  }

  if (m_currentVideoFormatUuid.is_nil()) {
    RGBVideoFormat rgbVideoFormat;
    return rgbVideoFormat;
  }

  VideoFormatConstIterator iterator(
      m_videoFormatsByUuid.find(m_currentVideoFormatUuid));
  if (iterator == m_videoFormatsByUuid.end()) {
    RGBVideoFormat rgbVideoFormat;
    return rgbVideoFormat;
  }

  const UuidVideoFormatPair uuidVideoFormatPair(*iterator);
  const RGBVideoFormat rgbVideoFormat(
      rgbVideoFormatFromPair(uuidVideoFormatPair));
  return rgbVideoFormat;
}

bool VideoCaptureDeviceImpl::setCurrentVideoFormat(
    const RGBVideoFormat& videoFormat) {
  if (!isInitialized()) {
    return false;
  }

  if (!videoFormat) {
    return false;
  }

  if (!stopCapturing()) {
    return false;
  }

  const Uuid videoFormatUuid(videoFormat.uuid());
  if (videoFormatUuid.is_nil()) {
    return false;
  }

  VideoFormatConstIterator iterator(
      m_videoFormatsByUuid.find(videoFormatUuid));
  if (iterator == m_videoFormatsByUuid.end()) {
    return false;
  }

  const UuidVideoFormatPair uuidVideoFormatPair(*iterator);
  const VideoFormatSharedPtr pVideoFormat(uuidVideoFormatPair.second);
  if (!pVideoFormat) {
    return false;
  }

  if (!pVideoFormat->setMediaTypeOfStream(m_pStreamConfig)) {
    return false;
  }

  return true;
}

static RGBVideoFormat firstVideoFormatFromList(
    VideoCaptureDeviceImpl::RGBVideoFormatList videoFormatList) {
  if (!videoFormatList.size()) {
    RGBVideoFormat videoFormat;
    return videoFormat;
  }
  RGBVideoFormat videoFormat(videoFormatList.front());
  return videoFormat;
}

} // VideoCapture
