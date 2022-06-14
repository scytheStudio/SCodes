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

#include <QThreadPool>
#include <QFuture>
#include <QtConcurrent>

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

    /*!
     * \fn QVideoSink *videoSink() const
     * \brief Function for getting sink of video output
     */
    QVideoSink *videoSink() const;

    /*!
     * \fn void setVideoSink(QVideoSink *videoSink)
     * \brief Function for setting sink of video output
     * \param const QVideoSink *videoSink - video sink
     */
    void setVideoSink(QVideoSink *videoSink);

    /*!
    * \fn QRectF captureRect() const
    * \brief Function for getting capture area
    */
    QRectF captureRect() const;

    /*!
     * \fn void setCaptureRect(const QRectF &captureRect)
     * \brief Function for setting capture area
     * \param const QRectF &captureRect - capture area
     */
    void setCaptureRect(const QRectF &captureRect);

    /*!
    * \fn QString captured() const
    * \brief Function for getting captured string
    */
    QString captured() const;

public slots:
    /*!
    * \fn void pauseProcessing()
    * \brief Function for pause image processing
    */
    void pauseProcessing();
    /*!
    * \fn void pauseProcessing()
    * \brief Function for continue image processing
    */
    void continueProcessing();

private:
    /*!
    * \brief Decoder instance
    */
    SBarcodeDecoder m_decoder;

    /*!
    * \brief Camera instance
    */
    QCamera *camera;

    /*!
    * \brief Pointer to a sink
    */
    QPointer<QVideoSink> m_videoSink;

    /*!
    * \brief Capture area
    */
    QRectF m_captureRect;

    /*!
    * \brief Captured string
    */
    QString m_captured = "";

    /*!
    * \brief Camera session instance
    */
    QMediaCaptureSession m_capture;

    /*!
    * \fn void setCaptured(const QString &captured)
    * \brief Function for setting capture string
    * \param const QString &captured - captured string
    */
    void setCaptured(const QString &captured);

    /*!
    * \fn void handleFrameCaptured(const QVideoFrame &frame)
    * \brief Function for handling video frame
    * \param const QVideoFrame &frame - video frame
    */
    void handleFrameCaptured(const QVideoFrame &frame);

signals:
    /*!
     * \brief This signal emitted when camera changed
     */
    void cameraChanged();

    /*!
     * \brief This signal is emitted when sink changed
     */
    void videoSinkChanged();

    /*!
     * \brief This signal is emitted when capture area changed
     * \param const QRectF &captureRect - capture area
     */
    void captureRectChanged(const QRectF &captureRect);

    /*!
     * \brief This signal is emitted when captured string changed
     * \param const QString &captured - captured string
     */
    void capturedChanged(const QString &captured);

private slots:
    /*!
    * \fn void imageProcess(const QVideoFrame &frame)
    * \brief Function for image processing
    * \param const QVideoFrame &frame - video frame
    */
    void imageProcess(const QVideoFrame &frame);

private slots:
    /*!
    * \fn void initCam()
    * \brief Function for initialization of camera
    */
    void initCam();
    /*!
    * \fn void stopCam()
    * \brief Function for stopping camera
    */
    void stopCam();

};

#endif // SBARCODESCANNER_H
