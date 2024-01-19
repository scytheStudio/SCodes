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

class Worker;

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
    /// Set this to the subsection of the frame that's acutally supposed to be scanned for qr code.
    Q_PROPERTY(QRectF captureRect READ captureRect WRITE setCaptureRect NOTIFY captureRectChanged)
    /// (Readonly) This string is set to the description of possible encountered error. Empty if no error.
    Q_PROPERTY(QString errorDescription READ errorDescription NOTIFY errorDescriptionChanged)
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
    QString errorDescription() const;
    void setCamera(QCamera *newCamera);
    void setForwardVideoSink(QVideoSink* sink);
public slots:
    /*!
     * \fn void pauseProcessing()
     * \brief Function for pause image processing
     */
    void pauseProcessing();

    /*!
     * \fn void continueProcessing()
     * \brief Function for continue image processing
     */
    void continueProcessing();

    /*!
     * \fn void setProcessing(bool p)
     * \brief Function for setting process status
     */
    void setProcessing(bool p);

    /*!
     * \fn void imageProcess(const QVideoFrame &frame)
     * \brief Function for image processing
     * \param const QVideoFrame &frame - video frame
     */
    void imageProcess(SBarcodeDecoder *decoder, const QImage &image, ZXing::BarcodeFormats formats);

signals:
    void cameraChanged(QCamera *);

    /// This signal emitted for running process in a thread
    void process(const QImage &image);
    void forwardVideoSinkChanged(QVideoSink*);
    void captureRectChanged(const QRectF &captureRect);
    void capturedChanged(const QString &captured);
    void cameraAvailableChanged();
    void errorDescriptionChanged();
protected:
    QCamera* makeDefaultCamera();
private:
    /// Decoder object, doing the actual Qr detection and conversion
    SBarcodeDecoder m_decoder;
    /// Camera object used to capture video, set automatically to device described by QMediaDevices::defaultVideoInput()
    QPointer<QCamera> m_camera;
    /// VideoSink to forward the captured frame to. Normally this should be VideoOutput.videoSink
    QPointer<QVideoSink> m_forwardVideoSink;
    /// Subsection of videoframe to capture, in pixels
    QRectF m_captureRect;
    /// Last captured string from QrCode
    QString m_captured = "";
    /// QMediaCaptureSession instance to actually perform the camera recording
    QMediaCaptureSession m_capture;
    /// Separate thread for Qr code processing and detection
    QThread workerThread;
    Worker *worker;

    bool m_processing = true;
    bool m_cameraAvailable = false;
    QString m_errorDescription = "";

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

    /*!
     * \fn void setErrorDescription(const QString &newErrorDescription)
     * \brief Function setting error message that occured during initialization
     * \param const QString &newErrorDescription - error message
     */
    void setErrorDescription(const QString &newErrorDescription);
};

/*!
 * \brief The Worker class allows you to run `SBarcodeScanner::imageProcess` method in a different thread
 */
class Worker : public QObject
{
    Q_OBJECT

private:
    /*!
     * \brief A pointer of SBarcodeScanner
     */
    SBarcodeScanner *_scanner;

public:
    /*!
     * \fn Worker(SBarcodeScanner *scanner)
     * \brief Constructor.
     * \param SBarcodeScanner *_scanner - a pointer to scanner class.
     */
    Worker(SBarcodeScanner *scanner) : _scanner{scanner} { ; }

public slots:

    /*!
    * \fn void process(const QImage &image)
    * \param const QImage &image - captured image
    * \brief An interface for processing image
    */
    void process(const QImage &image) {
        _scanner->imageProcess(_scanner->getDecoder(), image, SCodes::toZXingFormat(SCodes::SBarcodeFormat::Basic));
    }

};

#endif // SBARCODESCANNER_H
