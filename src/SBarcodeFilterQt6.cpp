#include "SBarcodeFilterQt6.h"

SBarcodeScannerQt6::SBarcodeScannerQt6(QObject *parent)
    : QVideoSink(parent)
    ,	camera(nullptr) {

    connect(&m_decoder, &SBarcodeDecoder::capturedChanged, this, &SBarcodeScannerQt6::setCaptured);
    connect(this, &QVideoSink::videoFrameChanged, this, &SBarcodeScannerQt6::handleFrameCaptured);

    worker = new Worker(this);
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &SBarcodeScannerQt6::process, worker, &Worker::process);
    workerThread.start();

    initCam();
}

SBarcodeScannerQt6::~SBarcodeScannerQt6()
{
    workerThread.quit();
    workerThread.wait();
    stopCam();
}

SBarcodeDecoder *SBarcodeScannerQt6::getDecoder()
{
    return &m_decoder;
}

void SBarcodeScannerQt6::initCam() {
    camera = new QCamera(this);

    const auto settings = camera->cameraDevice().videoFormats();

#ifdef Q_OS_ANDROID
    int i = camera->cameraDevice().videoFormats().size() - 1;
    const auto s = settings.at(i);
#else
    int i = 0;
    const auto s = settings.at(i);
#endif

    int w = settings.at(i).resolution().width();
    int h = settings.at(i).resolution().height();
    m_decoder.setResolution(w, h);

    camera->setFocusMode(QCamera::FocusModeAuto);
    camera->setCameraFormat(s);

    m_capture.setCamera(camera);
    m_capture.setVideoSink(this);

    camera->start();
}

void SBarcodeScannerQt6::stopCam()
{
    camera->stop();
    disconnect(camera, 0, 0, 0);
    camera->setParent(nullptr);
    delete camera;
    camera = nullptr;
}

void SBarcodeScannerQt6::handleFrameCaptured(const QVideoFrame &frame) {


    if(m_processing) {

        qDebug() << m_processing;

        emit process(m_decoder.videoFrameToImage(frame, captureRect().toRect()));

        if(m_videoSink) {
            m_videoSink->setVideoFrame(frame);
        }
    }
    pauseProcessing();
}

void SBarcodeScannerQt6::imageProcess(SBarcodeDecoder *decoder, const QImage &image, ZXing::BarcodeFormats formats) {

    decoder->process(image, formats);

    continueProcessing();

}

void SBarcodeScannerQt6::setProcessing(bool p) {
    m_processing = p;
}

void SBarcodeScannerQt6::pauseProcessing() {
    disconnect(this, &QVideoSink::videoFrameChanged, this, &SBarcodeScannerQt6::handleFrameCaptured);
}

void SBarcodeScannerQt6::continueProcessing() {
    connect(this, &QVideoSink::videoFrameChanged, this, &SBarcodeScannerQt6::handleFrameCaptured);
}

void SBarcodeScannerQt6::setCaptured(const QString &captured)
{
    if (m_captured == captured) {
        return;
    }
    m_captured = captured;
    emit capturedChanged(m_captured);
}

QString SBarcodeScannerQt6::captured() const
{
    return m_captured;
}

QRectF SBarcodeScannerQt6::captureRect() const {
    return m_captureRect;
}

void SBarcodeScannerQt6::setCaptureRect(const QRectF &captureRect)
{
    if (captureRect == m_captureRect) {
        return;
    }
    m_captureRect = captureRect;
    emit captureRectChanged(m_captureRect);
}

QVideoSink *SBarcodeScannerQt6::videoSink() const {
    return m_videoSink.get();
}

void SBarcodeScannerQt6::setVideoSink(QVideoSink *videoSink) {
    if (m_videoSink == videoSink) {
        return;
    }
    m_videoSink = videoSink;
    emit videoSinkChanged();
}
