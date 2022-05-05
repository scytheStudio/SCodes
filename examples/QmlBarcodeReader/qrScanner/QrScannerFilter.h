#ifndef QRSCANNERFILTER_H
#define QRSCANNERFILTER_H

#include <QAbstractVideoFilter>
#include <QtConcurrent/QtConcurrent>

#include "QrDecoder.h"

/*!
 * \brief Filter used to capture QR code from camera
 */
class QrScannerFilter : public QAbstractVideoFilter
{
    Q_OBJECT

    Q_PROPERTY(QRectF captureRect MEMBER _captureRect NOTIFY captureRectChanged)
    Q_PROPERTY(QString captured READ captured NOTIFY capturedChanged)
    Q_PROPERTY(bool isDecoding READ isDecoding NOTIFY isDecodingChanged)
    Q_PROPERTY(QString assayName READ assayName WRITE setAssayName NOTIFY assayNameChanged)

public:

    /*!
     * \fn QrScannerFilter()
     * \brief Constructor.
     */
    QrScannerFilter();

    /*!
     * \fn QString captured() const
     * \brief Returns the captured barcode string.
     */
    QString captured() const;

    /*!
     * \fn QrDecoder *getDecoder() const
     * \brief Returns the decoder.
     */
    QrDecoder *getDecoder() const;

    /*!
     * \fn QFuture<void> getFutureThread() const
     * \brief Returns the future thread.
     */
    QFuture<void> getFutureThread() const;

    /*!
     * \fn bool isDecoding() const
     * \brief Returns the decoding state.
     */
    bool isDecoding() const;

    /*!
     * \fn QVideoFilterRunnable *createFilterRunnable() override
     * \brief Returns instance of the SBarcodeFilterRunnable subclass.
     */
    QVideoFilterRunnable *createFilterRunnable() override;

    /*!
     * \fn QString assayName() const
     * \brief Returns the assay name.
     */
    QString assayName() const;

public slots:

    /*!
     * \fn void setAssayName(QString assayName);
     * \brief Sets the assay name.
     * \param QString assayName - assay name.
     */
    void setAssayName(QString assayName);

signals:

    /*!
     * \brief This signal is emitted when capture area rectangle changed.
     */
    void captureRectChanged();

    /*!
     * \brief This signal is emitted when captured barcode string changed.
     */
    void capturedChanged();

    /*!
     * \brief This signal is emitted to send decoding state to QML.
     * \param bool isDecoding - decoding state.
     */
    void isDecodingChanged(bool isDecoding);

    /*!
     * \brief This signal is emitted to send assay name to QML.
     * \param QString assayName - assay name.
     */
    void assayNameChanged(QString assayName);

private slots:

    /*!
     * \fn void setCaptured(const QString &captured)
     * \brief Sets captured barcode string.
     * \param const QString &captured - captured barcode string.
     */
    void setCaptured(const QString &captured);

private:
    QSharedPointer<QMutex> _mutexP;

    QRectF _captureRect;

    QString _captured;

    // QtEgoo::QtCapsuleQRData _capturedData;

    QrDecoder *_decoder;

    QFuture<void> _futureThread;

    bool _isDecoding;

    QString m_assayName;
};

#endif // QRSCANNERFILTER_H
