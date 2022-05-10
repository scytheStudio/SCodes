#ifndef QR_DECODER_H
#define QR_DECODER_H

#include <QObject>
#include <QVideoFrame>
#include <QOpenGLContext>

/*!
 * \brief Class used to convert video frame into image and scan QR code from it.
 */

class QrDecoder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString captured READ captured WRITE setCaptured NOTIFY capturedChanged)
    Q_PROPERTY(bool isDecoding READ isDecoding WRITE setIsDecoding NOTIFY isDecodingChanged)

public:

    /*!
     * \fn explicit QrDecoder(QObject *parent = nullptr)
     * \brief Constructor.
     * \param QObject *parent - a pointer to the parent object.
     */
    explicit QrDecoder(QObject *parent = nullptr);

    /*!
     * \fn QString captured() const
     * \brief Returns the captured barcode string.
     */
    QString captured() const;

    /*!
     * \fn bool isDecoding() const
     * \brief Returns the decoding state.
     */
    bool isDecoding() const;

    /*!
     * \fn QVideoFrame videoFrame() const
     * \brief Returns the video frame of data.
     */
    QVideoFrame videoFrame() const;

    /*!
     * \fn void setCtx(QOpenGLContext *ctx)
     * \brief Sets OpenGLContext.
     * \params QOpenGLContext *ctx - a pointer to opengl context.
     */
    void setCtx(QOpenGLContext *ctx);

    /*!
     * \fn void setVideoFrame(const QVideoFrame &videoFrame)
     * \brief Sets video frame of data.
     * \params const QVideoFrame &videoFrame - video frame of data.
     */
    void setVideoFrame(const QVideoFrame &videoFrame);

    /*!
     * \fn static QImage videoFrameToImage(QVideoFrame &videoFrame)
     * \brief Returns image from video frame.
     * \param QVideoFrame &videoFrame - frame of video data.
     */
    static QImage videoFrameToImage(const QVideoFrame &videoFrame);

public slots:

    /*!
     * \fn void process(const QImage capturedImage)
     * \brief Processes the image to scan the given barcode format types.
     * \param const QImage capturedImage - captured image.
     */
    void process(const QImage capturedImage);

signals:

    /*!
     * \brief This signal is emitted to send captured barcode string to QML.
     * \param const QString &captured - captured barcode string.
     */
    void capturedChanged(const QString &captured);

    /*!
     * \brief This signal is emitted to send decoding state to QML.
     * \param bool isDecoding - decoding state.
     */
    void isDecodingChanged(bool isDecoding);

private:
    QString _captured = "";

    QOpenGLContext *_ctx;

    bool _isDecoding = false;

    QVideoFrame _videoFrame;

    /*!
     * \fn void setCaptured(const QString &captured)
     * \brief Sets captured barcode string.
     * \param QString captured - captured barcode string.
     */
    void setCaptured(QString captured);

    /*!
     * \fn void setIsDecoding(bool isDecoding)
     * \brief Sets decoding state.
     * \param bool isDecoding - decoding state.
     */
    void setIsDecoding(bool isDecoding);
};

#endif // QR_DECODER_H
