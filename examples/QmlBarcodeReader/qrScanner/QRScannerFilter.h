#ifndef QRSCANNERFILTER_H
#define QRSCANNERFILTER_H

#include <QAbstractVideoFilter>
#include <QtConcurrent/QtConcurrent>

#include "QRDecoder.h"

/*!
 * \brief Filter used to capture QR code from camera
 */
class QRScannerFilter : public QAbstractVideoFilter
{
    Q_OBJECT

    Q_PROPERTY(QRectF captureRect MEMBER _captureRect NOTIFY captureRectChanged)
    Q_PROPERTY(QString captured READ captured NOTIFY capturedChanged)
    Q_PROPERTY(bool isDecoding READ isDecoding NOTIFY isDecodingChanged)
    Q_PROPERTY(QString assayName READ assayName WRITE setAssayName NOTIFY assayNameChanged)

public:
    QRScannerFilter();

    QString captured();
    QRDecoder *getDecoder() const;
    QFuture<void> getFutureThread() const;
    bool isDecoding() const;

    QVideoFilterRunnable * createFilterRunnable() override;

    QString assayName() const;

public slots:
    void setAssayName(QString assayName);

signals:
    void captureRectChanged();
    void capturedChanged();
    void isDecodingChanged(bool isDecoding);
    void assayNameChanged(QString assayName);

private slots:
    void setCaptured(const QString &captured);

private:
    QSharedPointer<QMutex> _mutexP;
    QRectF _captureRect;
    QString _captured;
    //QtEgoo::QtCapsuleQRData _capturedData;
    QRDecoder *_decoder;
    QFuture<void> _futureThread;
    bool _isDecoding;
    QString m_assayName;
};

#endif // QRSCANNERFILTER_H
