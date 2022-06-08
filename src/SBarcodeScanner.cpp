#include "SBarcodeScanner.h"

SBarcodeScanner::SBarcodeScanner(QObject *parent) {

    connect(&m_decoder, &SBarcodeDecoder::capturedChanged, this, &SBarcodeScanner::setCaptured);


    //threadPool = new QThreadPool(this);
    //threadPool->setMaxThreadCount(2);

}


void SBarcodeScanner::handleFrameCaptured(const QVideoFrame &frame) {

    if(imageFuture.isRunning() == false) {

        imageFuture = QtConcurrent::run(&SBarcodeScanner::imageProcess, this, frame);

        qDebug() << QDateTime::currentDateTime() << "|" << cnt++ << imageFuture.isRunning();
    }

}


void SBarcodeScanner::imageProcess(const QVideoFrame &frame) {

    const QImage img = m_decoder.videoFrameToImage(frame, captureRect().toRect());

    m_decoder.process(img, SCodes::toZXingFormat(SCodes::SBarcodeFormat::Basic));
}

void SBarcodeScanner::setCamera(QObject *cameraObject) {
    if (q_camera == cameraObject) {
        return;
    }

    if(m_capture) {
        m_capture->deleteLater();
    }

    q_camera = cameraObject;

    if(q_camera) {

        if(QCamera *camera = qobject_cast<QCamera *>(q_camera)) {

            m_capture = new QMediaCaptureSession(this);

            m_capture->setCamera(camera);
            m_imgcapture = new QImageCapture(this);
            m_capture->setImageCapture(m_imgcapture);

            m_capture->setVideoOutput(m_videoSink);
            connect(m_videoSink, &QVideoSink::videoFrameChanged, this, &SBarcodeScanner::handleFrameCaptured);

        } else {

            m_capture = nullptr;
        }
    emit cameraChanged();
   }
}

QObject *SBarcodeScanner::camera() const {
    return q_camera;
}

void SBarcodeScanner::capture() {
    m_imgcapture->capture();
}

void SBarcodeScanner::pauseProcessing() {
    disconnect(m_videoSink, &QVideoSink::videoFrameChanged, this, &SBarcodeScanner::handleFrameCaptured);
}

void SBarcodeScanner::continueProcessing() {
    connect(m_videoSink, &QVideoSink::videoFrameChanged, this, &SBarcodeScanner::handleFrameCaptured);
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
