#ifndef SBARCODESCANNER_H
#define SBARCODESCANNER_H

#include <QObject>
#include <QCamera>

#include <QImageCapture>
#include <QMediaCaptureSession>
#include <QVideoSink>

#include <QFuture>
#include <QtConcurrent>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include "SBarcodeDecoder.h"

class SBarcodeScanner : public QVideoSink
{
    Q_OBJECT
    Q_PROPERTY(QVideoSink* videoSink READ videoSink WRITE setVideoSink NOTIFY videoSinkChanged)
    Q_PROPERTY(QRectF captureRect READ captureRect WRITE setCaptureRect NOTIFY captureRectChanged)

public:
    explicit SBarcodeScanner(QObject *parent = nullptr);
    ~SBarcodeScanner() override;

    //! \return Sink of video output.
    QVideoSink *videoSink() const;
    //! Set sink of video output.
    void setVideoSink(QVideoSink *videoSink);

    //! \return Get capture area.
    QRectF captureRect() const;
    //! Set capture area.
    void setCaptureRect(const QRectF &captureRect);

    //! \return Captured string.
    QString captured() const;

public slots:
    //! Pause the image processing.
    void pauseProcessing();
    //! Continue the image processing.
    void continueProcessing();

private:
    //! Decoder instance.
    SBarcodeDecoder m_decoder;

    //! Camera instance.
    QCamera *camera;

    //! Pointer to a sink
    QPointer<QVideoSink> m_videoSink;

    //! Capture area.
    QRectF m_captureRect;

    //! Captured string.
    QString m_captured = "";

    //! Instance of capture session
    QMediaCaptureSession m_capture;

    //! Set captured string
    void setCaptured(const QString &captured);

    //! Image handle slot
    void handleFrameCaptured(const QVideoFrame &frame);

signals:
    //! Signal emitted when camera changed
    void cameraChanged();

    //! Signal emitted when sink changed
    void videoSinkChanged();

    //! Signal emitted when capture area changed
    void captureRectChanged(const QRectF &captureRect);

    //! Signal emitted when captured string changed
    void capturedChanged(const QString &captured);

private slots:
    //! Slot for image processing
    void imageProcess(const QVideoFrame &frame);

private slots:
    //! Init camera.
    void initCam();
    //! Stop camera.
    void stopCam();
    //! Video frame changed.

};

#endif // SBARCODESCANNER_H
