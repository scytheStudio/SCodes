#ifndef QRSCANNERFILTER_H
#define QRSCANNERFILTER_H

#include <QAbstractVideoFilter>
#include <QtConcurrent/QtConcurrent>
#include <qqml.h>

#include "./BarcodeDecoder.h"

class BarcodeFilter : public QAbstractVideoFilter
{
    Q_OBJECT
    Q_PROPERTY(QString captured READ captured NOTIFY capturedChanged)
    Q_PROPERTY(QRectF captureRect READ captureRect WRITE setCaptureRect NOTIFY captureRectChanged)
    QML_ELEMENT

public:
    explicit BarcodeFilter(QObject *parent = nullptr);

    QString captured() const;

    QRectF captureRect() const;
    void setCaptureRect(const QRectF &captureRect);

    BarcodeDecoder *getDecoder() const;
    QFuture<void> getFutureThread() const;

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

    BarcodeDecoder *_decoder;
    QFuture<void> _futureThread;
};

#endif // QRSCANNERFILTER_H
