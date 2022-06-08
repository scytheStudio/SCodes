#ifndef SBARCODESCANNER_H
#define SBARCODESCANNER_H

#include <QObject>
#include <QCamera>

#include <QImageCapture>
#include <QMediaCaptureSession>
#include <QVideoSink>

#include <QFuture>
#include <QThreadPool>
#include <QtConcurrent>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include "SBarcodeDecoder.h"

#include <QDateTime>


using namespace ZXing;

class SBarcodeScanner : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(QVideoSink* videoSink READ videoSink WRITE setVideoSink NOTIFY videoSinkChanged)
    Q_PROPERTY(QRectF captureRect READ captureRect WRITE setCaptureRect NOTIFY captureRectChanged)

public:

    int cnt=0;

    explicit SBarcodeScanner(QObject *parent = nullptr);

    QObject *camera() const;
    QVideoSink *videoSink() const;
    QRectF captureRect() const;
    QString captured() const;

    void capture();
    void setCamera(QObject *cameraObject);
    void setVideoSink(QVideoSink *videoSink);
    void setCaptureRect(const QRectF &captureRect);

public slots:
    void pauseProcessing();
    void continueProcessing();

private:
    SBarcodeDecoder m_decoder;

    QPointer<QObject> q_camera;
    QPointer<QVideoSink> m_videoSink;
    QRectF m_captureRect;
    QString m_captured = "";
    QFuture<void> imageFuture;
    QThreadPool *threadPool;

    QImageCapture *m_imgcapture     = nullptr;
    QMediaCaptureSession *m_capture = nullptr;

    void setCaptured(const QString &captured);
    void handleFrameCaptured(const QVideoFrame &frame);

signals:
    void cameraChanged();
    void videoSinkChanged();
    void imageChanged(const QImage &image);
    void captureRectChanged(const QRectF &captureRect);
    void capturedChanged(const QString &captured);

private slots:
    void imageProcess(const QVideoFrame &frame);
};

#endif // SBARCODESCANNER_H
