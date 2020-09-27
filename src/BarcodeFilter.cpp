#include "BarcodeFilter.h"

#include <QImage>
#include <QtMultimedia/qvideoframe.h>
#include <QVideoFilterRunnable>

#include "BarcodeDecoder.h"

void processImage(BarcodeDecoder *decoder, const QImage &image)
{
  decoder->process(image);
};

class BarcodeFilterRunnable : public QVideoFilterRunnable
{

public:

    BarcodeFilterRunnable(BarcodeFilter *filter)
        : _filter{filter}
    {
    }

    QVideoFrame run(QVideoFrame *input,
                    const QVideoSurfaceFormat &surfaceFormat,
                    QVideoFilterRunnable::RunFlags flags) override
    {
        Q_UNUSED(surfaceFormat);
        Q_UNUSED(flags);

        if (_filter->getDecoder()->isDecoding()) {
            return *input;
        }

        if (_filter->getImageFuture().isRunning()) {
            return *input;
        }

        const QImage croppedCapturedImage = BarcodeDecoder::videoFrameToImage(*input, _filter->captureRect().toRect());
        _filter->getImageFuture() = QtConcurrent::run(processImage, _filter->getDecoder(), croppedCapturedImage);

        return *input;
    }

private:
    BarcodeFilter *_filter;
};


BarcodeFilter::BarcodeFilter(QObject *parent)
    : QAbstractVideoFilter{parent}
    , _decoder{new BarcodeDecoder}
{
    connect(_decoder, &BarcodeDecoder::capturedChanged, this, &BarcodeFilter::setCaptured);

    connect(this, &QAbstractVideoFilter::activeChanged, this, [this]() {
        if (this->isActive()) {
            this->clean();
        }
    });
}

QVideoFilterRunnable *BarcodeFilter::createFilterRunnable()
{
    return new BarcodeFilterRunnable(this);
}

QString BarcodeFilter::captured() const
{
    return _captured;
}

void BarcodeFilter::setCaptured(const QString &captured)
{
    if (captured == _captured) {
        return;
    }

    _captured = captured;
    emit capturedChanged(_captured);
}

void BarcodeFilter::clean()
{
    _captured = "";
    _decoder->clean();
}

QRectF BarcodeFilter::captureRect() const
{
    return _captureRect;
}

void BarcodeFilter::setCaptureRect(const QRectF &captureRect)
{
    if (captureRect == _captureRect) {
        return;
    }

    _captureRect = captureRect;
    emit captureRectChanged(_captureRect);
}

BarcodeDecoder *BarcodeFilter::getDecoder() const
{
    return _decoder;
}

QFuture<void> BarcodeFilter::getImageFuture() const
{
    return _imageFuture;
}
