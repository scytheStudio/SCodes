#include "SBarcodeFilter.h"

#include <QImage>
#include <QtMultimedia/qvideoframe.h>
#include <QVideoFilterRunnable>

#include "SBarcodeDecoder.h"

void processImage(SBarcodeDecoder *decoder, const QImage &image, ZXing::BarcodeFormats formats)
{
    decoder->process(image, formats);
};

class SBarcodeFilterRunnable : public QVideoFilterRunnable
{
public:

    /*!
     * \fn SBarcodeFilterRunnable(SBarcodeFilter *filter)
     * \brief Constructor.
     * \param SBarcodeFilter *filter - a pointer to filter.
     */
    SBarcodeFilterRunnable(SBarcodeFilter *filter)
        : _filter{filter}
    { }

    /*!
     * \fn QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, QVideoFilterRunnable::RunFlags flags) override
     * \brief Run method in order to asynchronously process the input video frame.
     * \param QVideoFrame *input - a pointer to frame of video data.
     * \param const QVideoSurfaceFormat &surfaceFormat - the stream format of a video presentation surface.
     * \param QVideoFilterRunnable::RunFlags flags - typedef for QFlags<RunFlag>.
     */
    QVideoFrame run(QVideoFrame *    input,
      const QVideoSurfaceFormat      &surfaceFormat,
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

        const QImage croppedCapturedImage =
          SBarcodeDecoder::videoFrameToImage(*input, _filter->captureRect().toRect());
        _filter->getImageFuture() =
          QtConcurrent::run(processImage, _filter->getDecoder(), croppedCapturedImage,
            SCodes::toZXingFormat(_filter->format()));

        return *input;
    }

private:
    SBarcodeFilter *_filter;
};


SBarcodeFilter::SBarcodeFilter(QObject *parent)
    : QAbstractVideoFilter{parent},
    _decoder{new SBarcodeDecoder}
{
    connect(_decoder, &SBarcodeDecoder::capturedChanged, this, &SBarcodeFilter::setCaptured);

    connect(this, &QAbstractVideoFilter::activeChanged, this, [this](){
        if (this->isActive()) {
            this->clean();
        }
    });
}

QVideoFilterRunnable *SBarcodeFilter::createFilterRunnable()
{
    return new SBarcodeFilterRunnable(this);
}

QString SBarcodeFilter::captured() const
{
    return _captured;
}

void SBarcodeFilter::setCaptured(const QString &captured)
{
    if (captured == _captured) {
        return;
    }

    _captured = captured;

    emit capturedChanged(_captured);
}

void SBarcodeFilter::clean()
{
    _captured = "";

    _decoder->clean();
}

QRectF SBarcodeFilter::captureRect() const
{
    return _captureRect;
}

void SBarcodeFilter::setCaptureRect(const QRectF &captureRect)
{
    if (captureRect == _captureRect) {
        return;
    }

    _captureRect = captureRect;

    emit captureRectChanged(_captureRect);
}

SBarcodeDecoder *SBarcodeFilter::getDecoder() const
{
    return _decoder;
}

QFuture<void> SBarcodeFilter::getImageFuture() const
{
    return _imageFuture;
}

const SCodes::SBarcodeFormats &SBarcodeFilter::format() const
{
    return _format;
}

void SBarcodeFilter::setFormat(const SCodes::SBarcodeFormats &format)
{
    qDebug() << "set format " << format << ", old format " << _format;

    if (_format != format) {
        _format = format;
        emit formatChanged(_format);
    }
}
