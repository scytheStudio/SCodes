#ifndef QR_DECODER_H
#define QR_DECODER_H

#include <QObject>
#include <QVideoFrame>

#include "SBarcodeFormat.h"

/*!
 * \brief The SBarcodeDecoder class for decoding barcodes to human readable string
 */
class SBarcodeDecoder : public QObject
{
    Q_OBJECT

public:

    /*!
     * \fn explicit SBarcodeDecoder(QObject *parent)
     * \brief Constructor.
     * \param QObject *parent - a pointer to the parent object.
     */
    explicit SBarcodeDecoder(QObject *parent = nullptr);

    /*!
     * \fn void clean()
     * \brief Sets the captured barcode string to empty.
     */
    void clean();

    /*!
     * \fn bool isDecoding() const
     * \brief Returns the decoding state.
     */
    bool isDecoding() const;

    /*!
     * \fn QString captured() const
     * \brief Returns the captured barcode string.
     */
    QString captured() const;

    /*!
     * \fn static QImage videoFrameToImage(QVideoFrame &videoFrame, const QRect &captureRect)
     * \brief Returns image from video frame.
     * \param QVideoFrame &videoFrame - frame of video data.
     * \param const QRect &captureRect - capture area rectangle.
     */
    static QImage videoFrameToImage(QVideoFrame &videoFrame, const QRect &captureRect);

    /*!
     * \fn static QImage imageFromVideoFrame(const QVideoFrame &videoFrame)
     * \brief Returns image from video frame.
     * \param const QVideoFrame &videoFrame - frame of video data.
     */
    static QImage imageFromVideoFrame(const QVideoFrame &videoFrame);

public slots:

    /*!
     * \fn void process(const QImage capturedImage, ZXing::BarcodeFormats formats)
     * \brief Processes the image to scan the given barcode format types.
     * \param const QImage capturedImage - captured image.
     * \param ZXing::BarcodeFormats formats - barcode formats.
     */
    void process(const QImage capturedImage, ZXing::BarcodeFormats formats);

signals:

    /*!
     * \brief This signal is emitted to send decoding state to QML.
     * \param bool isDecoding - decoding state.
     */
    void isDecodingChanged(bool isDecoding);

    /*!
     * \brief This signal is emitted to send captured barcode string to QML.
     * \param const QString &captured - captured barcode string.
     */
    void capturedChanged(const QString &captured);

private:
    bool _isDecoding = false;

    QString _captured = "";

    /*!
     * \fn void setCaptured(const QString &captured)
     * \brief Sets captured barcode string.
     * \param const QString &captured - captured barcode string.
     */
    void setCaptured(const QString &captured);

    /*!
     * \fn void setIsDecoding(bool isDecoding)
     * \brief Sets decoding state.
     * \param bool isDecoding - decoding state.
     */
    void setIsDecoding(bool isDecoding);
};

#endif // QR_DECODER_H
