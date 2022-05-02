#ifndef QRSCANNERFILTER_H
#define QRSCANNERFILTER_H

#include <QAbstractVideoFilter>
#include <QtConcurrent/QtConcurrent>
#include <qqml.h>

#include "SBarcodeDecoder.h"
#include "SBarcodeFormat.h"

/*!
 * \brief The SBarcodeFilter class is a custom class that allows image processing with the cooperation of QML VideoOutput type.
 */
class SBarcodeFilter : public QAbstractVideoFilter
{
    Q_OBJECT
    Q_PROPERTY(QString captured READ captured NOTIFY capturedChanged)
    Q_PROPERTY(QRectF captureRect READ captureRect WRITE setCaptureRect NOTIFY captureRectChanged)
    Q_PROPERTY(SCodes::SBarcodeFormats format READ format WRITE setFormat NOTIFY formatChanged)
    #if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    QML_ELEMENT
    #endif

public:

    /*!
     * \fn explicit SBarcodeFilter(QObject *parent)
     * \brief Constructor.
     * \param QObject *parent - a pointer to the parent object.
     */
    explicit SBarcodeFilter(QObject *parent = nullptr);

    /*!
     * \fn QString captured() const
     * \brief Returns the captured barcode string.
     */
    QString captured() const;

    /*!
     * \fn QRectF captureRect() const
     * \brief Returns the capture area rectangle.
     */
    QRectF captureRect() const;

    /*!
     * \fn void setCaptureRect(const QRectF &captureRect)
     * \brief Sets the capture area rectangle.
     * \param const QRectF &captureRect - capture area rectagle.
     */
    void setCaptureRect(const QRectF &captureRect);

    /*!
     * \fn SBarcodeDecoder *getDecoder() const
     * \brief Returns the decoder.
     */
    SBarcodeDecoder *getDecoder() const;

    /*!
     * \fn QFuture<void> getImageFuture() const
     * \brief Returns the future thread.
     */
    QFuture<void> getImageFuture() const;

    /*!
     * \fn QVideoFilterRunnable *createFilterRunnable() override
     * \brief Returns instance of the SBarcodeFilterRunnable subclass.
     */
    QVideoFilterRunnable *createFilterRunnable() override;

    /*!
     * \fn const SCodes::SBarcodeFormats &format() const
     * \brief Returns the barcode format.
     */
    const SCodes::SBarcodeFormats &format() const;

    /*!
     * \fn void setFormat(SCodes::SBarcodeFormat format)
     * \brief Sets the barcode format.
     * \param SCodes::SBarcodeFormat format - barcode format.
     */
    void setFormat(const SCodes::SBarcodeFormats &format);

signals:

    /*!
     * \brief This signal is emitted to send captured barcode string to QML.
     * \param const QString &captured - captured barcode string.
     */
    void capturedChanged(const QString &captured);

    /*!
     * \brief This signal is emitted to send capture area rectangle to QML.
     * \param const QRectF &captureRect - capture area rectangle.
     */
    void captureRectChanged(const QRectF &captureRect);

    /*!
     * \brief This signal is emitted to send barcode format to QML.
     * \param const SCodes::SBarcodeFormat &format - barcode format.
     */
    void formatChanged(const SCodes::SBarcodeFormats &format);

private slots:

    /*!
     * \fn void setCaptured(const QString &captured)
     * \brief Sets the captured barcode string.
     * \param const QString &captured - captured barcode string.
     */
    void setCaptured(const QString &captured);

    /*!
     * \fn void clean()
     * \brief Sets the captured barcode string to empty.
     */
    void clean();

private:
    QString _captured = "";

    QRectF _captureRect;

    SBarcodeDecoder *_decoder;

    QFuture<void> _imageFuture;

    SCodes::SBarcodeFormats _format = SCodes::SBarcodeFormat::Basic;
};

#endif // QRSCANNERFILTER_H
