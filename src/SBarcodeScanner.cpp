#include "SBarcodeScanner.h"
#include <QMediaDevices>
#include "private/debug.h"
SBarcodeScanner::SBarcodeScanner(QObject* parent)
    : QVideoSink(parent)
    , m_camera(nullptr)
    , m_scanning{true}
{
    // Print error message if error occurs
    connect(this, &SBarcodeScanner::errorOccured, this, [](const QString& msg){
        qWarning() << "SCodes Error:" << msg;
    });
    // Connect self to the media capture session
    m_capture.setVideoSink(this);
    connect(this, &QVideoSink::videoFrameChanged, this, &SBarcodeScanner::tryProcessFrame);

    // Connect cameraAvaliable property. Utilise implicit conversion from pointer to bool.
    connect(this, &SBarcodeScanner::cameraChanged, this, &SBarcodeScanner::setCameraAvailable);

    
    m_decoder.moveToThread(&workerThread);
    connect(&m_decoder, &SBarcodeDecoder::capturedChanged, this, &SBarcodeScanner::setCaptured, Qt::QueuedConnection);
    connect(&m_decoder, &SBarcodeDecoder::errorOccured, this, &SBarcodeScanner::errorOccured, Qt::QueuedConnection);
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
    if(!m_scanning || m_frameProcessingInProgress) {
        return;
    }
    // Set the guard variable to not process more than 1 frame at the time
    m_frameProcessingInProgress = true;

    // Scale the normalized rectangle for camera resolution
    auto r = m_camera->cameraFormat().resolution();
    auto cRect = QRectF{m_captureRect.x()*r.width(),
            m_captureRect.y()*r.height(),
            m_captureRect.width()*r.width(),
                       m_captureRect.height()*r.height()}.toRect();

    // Invoke processing asynchronously, potential result will be reported by capturedChanged signal
    // We can copy QVideoFrame as it's explicitly shared (just like std::shared_ptr)
    // Note the releasing the guard variable
    QMetaObject::invokeMethod(&m_decoder, [=](){
        m_decoder.process(m_decoder.videoFrameToImage(frame, cRect),SCodes::toZXingFormat(SCodes::SBarcodeFormat::Basic));
        m_frameProcessingInProgress = false;
    });
}

void SBarcodeScanner::setCameraAvailable(bool available)
{
    if (m_cameraAvailable == available) {
        return;
    }

    m_cameraAvailable = available;
    emit cameraAvailableChanged();
}

QCamera *SBarcodeScanner::makeDefaultCamera()
{
    auto defaultCamera = QMediaDevices::defaultVideoInput();
    if (defaultCamera.isNull())
    {
        errorOccured("No default camera could be found on the system");
        return nullptr;
    }

    auto camera = new QCamera(defaultCamera, this);
    if (camera->error()) {
        errorOccured("Error during camera initialization: " + camera->errorString());
        return nullptr;
    }

    auto supportedFormats = camera->cameraDevice().videoFormats();
    for (auto f : supportedFormats)
    {
        sDebug() << "Supported format: "
                 << f.pixelFormat()
                 << f.resolution()
                 << "  FPS:" << f.minFrameRate() << "-" << f.maxFrameRate();
    }
    if(supportedFormats.empty())
    {
        errorOccured("A default camera was found but it has no supported formats. The Camera may be wrongly configured.");
        return nullptr;
    }

    /// Pick best format - most pixels
    std::sort(supportedFormats.begin(),supportedFormats.end(),[](const auto& f1, const auto& f2){
        QSize r1 = f1.resolution();
        QSize r2 = f2.resolution();
        return r1.height()*r1.width() < r2.height()*r2.width();
    });
    auto format = supportedFormats.last();

    camera->setFocusMode(QCamera::FocusModeAuto);
    camera->setCameraFormat(format);
    return camera;
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
            errorOccured("Camera error:" + string);
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
