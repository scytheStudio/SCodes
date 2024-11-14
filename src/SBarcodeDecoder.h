#ifndef QR_DECODER_H
#define QR_DECODER_H

#include <QObject>
#include <QVideoFrame>

#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include "SBarcodeFormat.h"

// Default camera resolution width/height
#define DEFAULT_RES_W 1080
#define DEFAULT_RES_H 1920

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
    QImage videoFrameToImage(const QVideoFrame &videoFrame, const QRect &captureRect) const;

    /*!
     * \fn void setResolution(const int &w, const int &h)
     * \brief Update camera resolution values
     * \param w - width of the resolution
     * \param h - height of the resolution
     */
    void setResolution(const QSize&);
    [[deprecated("Use QSize overload instead")]] void setResolution(int w, int h);

public slots:
    /*!
     * \fn void process(const QImage capturedImage, ZXing::BarcodeFormats formats)
     * \brief Processes the image to scan the given barcode format types.
     * \param const QImage capturedImage - captured image.
     * \param ZXing::BarcodeFormats formats - barcode formats.
     */
    void process(const QImage& capturedImage, ZXing::BarcodeFormats formats);

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

    void errorOccured(const QString& errorString);

private:
    /*!
     * \brief Indicates the decoding state
     */
    bool m_isDecoding = false;

    /*!
     * \brief Captured string from barcode
     */
    QString m_captured = "";
    QSize m_resolution;

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
