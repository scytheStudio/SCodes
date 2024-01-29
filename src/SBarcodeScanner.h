#ifndef SBARCODESCANNER_H
#define SBARCODESCANNER_H

#include <QObject>
#include <QCamera>
#include <QThread>
#include <QImageCapture>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include "SBarcodeDecoder.h"
/*!
 * \brief The SBarcodeScanner class processes the video input from Camera,
 */
class SBarcodeScanner : public QVideoSink, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    QML_ELEMENT

    /// Set this property to the videosink that's supposed to do further processing on video frame. A VideoOutput.videosink for example, to show the video.
    Q_PROPERTY(QVideoSink* forwardVideoSink MEMBER m_forwardVideoSink WRITE setForwardVideoSink NOTIFY forwardVideoSinkChanged)
    /// This property controls wether the frames are passed along to the decoder or not (default true)
    Q_PROPERTY(bool scanning MEMBER m_scanning)
    /// Set this to the subsection of the frame that's acutally supposed to be scanned for qr code. In Normalized coordinates (0.0-1.0)
    Q_PROPERTY(QRectF captureRect READ captureRect WRITE setCaptureRect NOTIFY captureRectChanged)
    /// Set to true if camera property is set
    Q_PROPERTY(bool cameraAvailable READ cameraAvailable NOTIFY cameraAvailableChanged)
    /// Optional property if you want to set your own camera as an video input for scanning. Default video input is chosen by default.
    Q_PROPERTY(QCamera* camera MEMBER m_camera WRITE setCamera NOTIFY cameraChanged)

public:
    explicit SBarcodeScanner(QObject *parent = nullptr);
    ~SBarcodeScanner() override;

    SBarcodeDecoder *getDecoder();
    /*!
     * This function is called after all properties set in Qml instantiation have been assigned.
     * This allows us to, for example create default camera if none has been set by the user
     */
    void componentComplete() override;
    /// This function does nothing and is here only to satisfy QQmlParserStatus interface
    void classBegin() override;



    QRectF captureRect() const;
    void setCaptureRect(const QRectF &captureRect);
    QString captured() const;
    bool cameraAvailable() const;
    void setCamera(QCamera *newCamera);
    void setForwardVideoSink(QVideoSink* sink);
public slots:

signals:
    void cameraChanged(QCamera *);

    /// This signal emitted for running process in a thread
    void process(const QImage &image);
    void forwardVideoSinkChanged(QVideoSink*);
    void captureRectChanged(const QRectF &captureRect);
    void capturedChanged(const QString &captured);
    void cameraAvailableChanged();
    void errorOccured(const QString& errorString);
protected:
    QCamera* makeDefaultCamera();
private:
    /// Decoder object, doing the actual Qr detection and conversion
    SBarcodeDecoder m_decoder;
    /// Camera object used to capture video, set automatically to device described by QMediaDevices::defaultVideoInput()
    QPointer<QCamera> m_camera;
    /// VideoSink to forward the captured frame to. Normally this should be VideoOutput.videoSink
    QPointer<QVideoSink> m_forwardVideoSink;
    /// Subsection of videoframe to capture, in normalized coordinates
    QRectF m_captureRect;
    /// Last captured string from QrCode
    QString m_captured = "";
    /// QMediaCaptureSession instance to actually perform the camera recording
    QMediaCaptureSession m_capture;
    /// Separate thread for Qr code processing and detection
    QThread workerThread;
    /// Guard variable that prevents us from queueuing up the frames for processing
    QAtomicInteger<bool> m_frameProcessingInProgress=false;

    bool m_scanning = true;
    bool m_cameraAvailable = false;

    /*!
     * \fn void setCaptured(const QString &captured)
     * \brief Function for setting capture string
     * \param const QString &captured - captured string
     */
    void setCaptured(const QString &captured);
    /// Try process captured frame, if previous frame is already processed - skip it
    void tryProcessFrame(const QVideoFrame &frame);

    /*!
     * \fn void setCameraAvailable(bool available)
     * \brief Function setting camera availability
     * \param bool available - camera availability status
     */
    void setCameraAvailable(bool available);
};

#endif // SBARCODESCANNER_H
