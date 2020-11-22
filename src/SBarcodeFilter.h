#ifndef QRSCANNERFILTER_H
#define QRSCANNERFILTER_H

#include <QAbstractVideoFilter>
#include <QtConcurrent/QtConcurrent>
#include <qqml.h>

#include "./SBarcodeDecoder.h"

class SBarcodeFilter : public QAbstractVideoFilter
{
    Q_OBJECT
    Q_PROPERTY(QString captured READ captured NOTIFY capturedChanged)
    Q_PROPERTY(QRectF captureRect READ captureRect WRITE setCaptureRect NOTIFY captureRectChanged)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
        QML_ELEMENT
#endif

public:
    explicit SBarcodeFilter(QObject *parent = nullptr);

    QString captured() const;

    QRectF captureRect() const;
    void setCaptureRect(const QRectF &captureRect);

    SBarcodeDecoder *getDecoder() const;
    QFuture<void> getImageFuture() const;

    QVideoFilterRunnable * createFilterRunnable() override;

signals:
    void capturedChanged(const QString &captured);
    void captureRectChanged(const QRectF &captureRect);

private slots:
    void setCaptured(const QString &captured);
    void clean();

private:
    QString _captured = "";
    QRectF _captureRect;

    SBarcodeDecoder *_decoder;
    QFuture<void> _imageFuture;
};

#endif // QRSCANNERFILTER_H
