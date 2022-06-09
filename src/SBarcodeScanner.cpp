#include "SBarcodeScanner.h"

SBarcodeScanner::SBarcodeScanner(QObject *parent)
    : QVideoSink( parent )
    ,	camera( nullptr ) {

    connect(&m_decoder, &SBarcodeDecoder::capturedChanged, this, &SBarcodeScanner::setCaptured);
    connect(this, &QVideoSink::videoFrameChanged, this, &SBarcodeScanner::handleFrameCaptured);

    initCam();

}

SBarcodeScanner::~SBarcodeScanner()
{
    stopCam();
}

void SBarcodeScanner::initCam() {
    camera = new QCamera(this);

    const auto settings = camera->cameraDevice().videoFormats();

    int i = 0;
    const auto s = settings.at(i);

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

    imageProcess(frame);

    if(m_videoSink) {
        m_videoSink->setVideoFrame(frame);
    }
}

void SBarcodeScanner::imageProcess(const QVideoFrame &frame) {

    const QImage img = m_decoder.videoFrameToImage(frame, captureRect().toRect());
    m_decoder.process(img, SCodes::toZXingFormat(SCodes::SBarcodeFormat::Basic));
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
