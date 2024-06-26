#include "SBarcodeFilter.h"

#include <QImage>
#include <QtMultimedia/qvideoframe.h>
#include <QVideoFilterRunnable>

#include "SBarcodeDecoder.h"
#include "private/debug.h"

void processImage(SBarcodeDecoder *decoder, const QImage &image, ZXing::BarcodeFormats formats)
{
    decoder->process(image, formats);
}

/*!
 * \brief Inherited from QVideoFilterRunnable class and provide `SBarcodeFilterRunnable::run` method in order to asynchronously process the input video frame
 */
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
            _filter->getDecoder()->videoFrameToImage(*input, _filter->captureRect().toRect());
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
    sDebug() << "FILTER CREATED!";
    return new SBarcodeFilterRunnable(this);
}

QString SBarcodeFilter::captured() const
{
    return m_captured;
}

void SBarcodeFilter::setCaptured(const QString &captured)
{
    if (captured == m_captured) {
        return;
    }

    m_captured = captured;

    emit capturedChanged(m_captured);
}

void SBarcodeFilter::clean()
{
    m_captured = "";

    _decoder->clean();
}

QRectF SBarcodeFilter::captureRect() const
{
    return m_captureRect;
}

void SBarcodeFilter::setCaptureRect(const QRectF &captureRect)
{
    if (captureRect == m_captureRect) {
        return;
    }

    m_captureRect = captureRect;

    emit captureRectChanged(m_captureRect);
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
    return m_format;
}

void SBarcodeFilter::setFormat(const SCodes::SBarcodeFormats &format)
{
    sDebug() << "set format " << format << ", old format " << m_format;

    if (m_format != format) {
        m_format = format;
        emit formatChanged(m_format);
    }
}
