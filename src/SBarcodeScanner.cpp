#include "SBarcodeScanner.h"

SBarcodeScanner::SBarcodeScanner(QObject *parent)
    : QVideoSink(parent)
    ,	camera(nullptr) {

    connect(&m_decoder, &SBarcodeDecoder::capturedChanged, this, &SBarcodeScanner::setCaptured);
    connect(this, &QVideoSink::videoFrameChanged, this, &SBarcodeScanner::handleFrameCaptured);

    worker = new Worker(this);
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &SBarcodeScanner::process, worker, &Worker::process);
    workerThread.start();

    initCam();
}

SBarcodeScanner::~SBarcodeScanner()
{
    workerThread.quit();
    workerThread.wait();
    stopCam();
}

SBarcodeDecoder *SBarcodeScanner::getDecoder()
{
    return &m_decoder;
}

void SBarcodeScanner::initCam() {
    camera = new QCamera(this);

    if(camera->error())
    {
        qDebug() << "[SCodes] initCam() Failed: " << camera->errorString();
        return;
    }

    const auto settings = camera->cameraDevice().videoFormats();

#ifdef Q_OS_ANDROID
    int i = camera->cameraDevice().videoFormats().size() - 1;
#else
    int i = 0;
#endif

    const auto s = settings.at(i);

    int w = settings.at(i).resolution().width();
    int h = settings.at(i).resolution().height();
    m_decoder.setResolution(w, h);

    camera->setFocusMode(QCamera::FocusModeAuto);
    camera->setCameraFormat(s);

    m_capture.setCamera(camera);
    m_capture.setVideoSink(this);

    camera->start();
}

void SBarcodeScanner::stopCam()
{
    camera->stop();
    disconnect(camera, 0, 0, 0);
    camera->setParent(nullptr);
    delete camera;
    camera = nullptr;
}

void SBarcodeScanner::handleFrameCaptured(const QVideoFrame &frame) {
    if(m_processing) {
        emit process(m_decoder.videoFrameToImage(frame, captureRect().toRect()));

        if(m_videoSink) {
            m_videoSink->setVideoFrame(frame);
        }
    }
    pauseProcessing();
}

void SBarcodeScanner::imageProcess(SBarcodeDecoder *decoder, const QImage &image, ZXing::BarcodeFormats formats) {
    decoder->process(image, formats);
    continueProcessing();
}

void SBarcodeScanner::setProcessing(bool p) {
    m_processing = p;
}

void SBarcodeScanner::pauseProcessing() {
    disconnect(this, &QVideoSink::videoFrameChanged, this, &SBarcodeScanner::handleFrameCaptured);
}

void SBarcodeScanner::continueProcessing() {
    connect(this, &QVideoSink::videoFrameChanged, this, &SBarcodeScanner::handleFrameCaptured);
}

void SBarcodeScanner::setCaptured(const QString &captured)
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

QRectF SBarcodeScanner::captureRect() const {
    return m_captureRect;
}

void SBarcodeScanner::setCaptureRect(const QRectF &captureRect)
{
    if (captureRect == m_captureRect) {
        return;
    }

    m_captureRect = captureRect;
    emit captureRectChanged(m_captureRect);
}

QVideoSink *SBarcodeScanner::videoSink() const {
    return m_videoSink.get();
}

void SBarcodeScanner::setVideoSink(QVideoSink *videoSink) {
    if (m_videoSink == videoSink) {
        return;
    }

    m_videoSink = videoSink;
    emit videoSinkChanged();
}
