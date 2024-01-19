#include "SBarcodeScanner.h"
#include <QMediaDevices>
#include "debug.h"
SBarcodeScanner::SBarcodeScanner(QObject* parent)
    : QVideoSink(parent)
    , m_camera(nullptr)
{
    // Connect self to the media capture session
    m_capture.setVideoSink(this);

    // initiate decoder on another thread
    connect(&m_decoder, &SBarcodeDecoder::capturedChanged, this, &SBarcodeScanner::setCaptured);
    connect(this, &QVideoSink::videoFrameChanged, this, &SBarcodeScanner::tryProcessFrame);

    // Connect cameraAvaliable property. Utilise implicit conversion from pointer to bool.
    connect(this, &SBarcodeScanner::cameraChanged, this, &SBarcodeScanner::cameraAvailable);

    worker = new Worker(this);
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &SBarcodeScanner::process, worker, &Worker::process);
    workerThread.start();
}

SBarcodeScanner::~SBarcodeScanner()
{
    workerThread.quit();
    workerThread.wait();
}

SBarcodeDecoder* SBarcodeScanner::getDecoder()
{
    return &m_decoder;
}

void SBarcodeScanner::componentComplete()
{
    sDebug() << "Component complete, Camera status: " << m_camera;
    if(m_camera.isNull())
    {
        sDebug() << "No camera provided. Setting default camera";
        setCamera(makeDefaultCamera());
    }
}

void SBarcodeScanner::classBegin()
{
    return;
}

void SBarcodeScanner::tryProcessFrame(const QVideoFrame& frame)
{
    if (m_processing) {
        emit process(m_decoder.videoFrameToImage(frame, captureRect().toRect()));
    }
    pauseProcessing();
}

void SBarcodeScanner::setCameraAvailable(bool available)
{
    if (m_cameraAvailable == available) {
        return;
    }

    m_cameraAvailable = available;
    emit cameraAvailableChanged();
}

void SBarcodeScanner::imageProcess(
    SBarcodeDecoder* decoder, const QImage& image, ZXing::BarcodeFormats formats)
{
    decoder->process(image, formats);
    continueProcessing();
}

QCamera *SBarcodeScanner::makeDefaultCamera()
{
    auto defaultCamera = QMediaDevices::defaultVideoInput();
    if (defaultCamera.isNull())
    {
        sDebug() << "No default camera found!";
        setErrorDescription("No default camera could be found on the system");
        return nullptr;
    }

    auto camera = new QCamera(defaultCamera, this);
    if (camera->error()) {
        sDebug() << "Error during camera initialization: " << camera->errorString();
        setErrorDescription(camera->errorString());
        return nullptr;
    }

// TODO: Either explain what this is or remove it
#ifdef Q_OS_ANDROID
    const auto format = camera->cameraDevice().videoFormats().last();
#else
    const auto format = camera->cameraDevice().videoFormats().first();
#endif

    camera->setFocusMode(QCamera::FocusModeAuto);
    camera->setCameraFormat(format);
    return camera;
}

void SBarcodeScanner::setProcessing(bool p)
{
    m_processing = p;
}

void SBarcodeScanner::pauseProcessing()
{
    disconnect(this, &QVideoSink::videoFrameChanged, this, &SBarcodeScanner::tryProcessFrame);
}

void SBarcodeScanner::continueProcessing()
{
    connect(this, &QVideoSink::videoFrameChanged, this, &SBarcodeScanner::tryProcessFrame);
}

void SBarcodeScanner::setCaptured(const QString& captured)
{
    if (m_captured == captured) {
        return;
    }

    m_captured = captured;
    emit capturedChanged(m_captured);
}

QString SBarcodeScanner::captured() const
{
    return m_captured;
}

QRectF SBarcodeScanner::captureRect() const
{
    return m_captureRect;
}

void SBarcodeScanner::setCaptureRect(const QRectF& captureRect)
{
    if (captureRect == m_captureRect) {
        return;
    }

    m_captureRect = captureRect;
    sDebug() << "Capture Rectangle changed:" << m_captureRect;
    emit captureRectChanged(m_captureRect);
}


bool SBarcodeScanner::cameraAvailable() const
{
    return m_cameraAvailable;
}

QString SBarcodeScanner::errorDescription() const
{
    return m_errorDescription;
}

void SBarcodeScanner::setCamera(QCamera *newCamera)
{
    if(m_camera == newCamera){
        return;
    }
    // disconnect old camera if not already null
    if(m_camera)
    {
        m_camera->stop();
        disconnect(m_camera,nullptr,this,nullptr);
        m_capture.setCamera(nullptr);
        if(m_camera->parent() == this)
        {
            delete m_camera;
        }
        m_camera = nullptr;
    }

    // connect new camera if not null
    if(newCamera)
    {
        auto format = newCamera->cameraFormat();
        connect(newCamera,&QCamera::errorOccurred,this,[this](auto err,const auto& string){
            sDebug() << "Camera error:" << string;
            setErrorDescription(string);
        });
        m_decoder.setResolution(format.resolution());
        m_capture.setCamera(newCamera);
        m_camera = newCamera;
        m_camera->start();
    }
    sDebug() << "New Camera set: " << m_camera
            << "\nFormat:\n"
            << "  Pixel format:" << m_camera->cameraFormat().pixelFormat() << '\n'
            << "  Resolution:" << m_camera->cameraFormat().resolution() << '\n'
            << "  FPS:" << m_camera->cameraFormat().minFrameRate() << "-" << m_camera->cameraFormat().maxFrameRate();

    emit cameraChanged(m_camera);
}

void SBarcodeScanner::setForwardVideoSink(QVideoSink *newSink)
{
    if(m_forwardVideoSink == newSink){
        return;
    }
    if(m_forwardVideoSink)
    {
        disconnect(this,nullptr,m_forwardVideoSink,nullptr);
    }
    if(newSink)
    {
        connect(this,&SBarcodeScanner::videoFrameChanged,newSink,&QVideoSink::setVideoFrame);
    }
    m_forwardVideoSink = newSink;
    forwardVideoSinkChanged(m_forwardVideoSink);
}

void SBarcodeScanner::setErrorDescription(const QString& newErrorDescription)
{
    if (m_errorDescription == newErrorDescription) {
        return;
    }

    m_errorDescription = newErrorDescription;
    emit errorDescriptionChanged();
}
