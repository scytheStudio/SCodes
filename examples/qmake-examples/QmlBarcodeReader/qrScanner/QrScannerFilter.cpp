#include "QrScannerFilter.h"
#include "QrDecoder.h"

#include <QVideoFilterRunnable>
#include <QFuture>
#include <QtMultimedia/qvideoframe.h>
#include <QImage>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QDebug>

void processImage(QrDecoder *decoder, const QImage &image)
{
    decoder->process(image);
};

/// This is linked to the private implementations of Qt. Check, when using a new versino of Qt that it still
/// works. E.g. by saving the images to disk and visually inspect them
extern QImage qt_imageFromVideoFrame(const QVideoFrame &frame);
class QRRunnable : public QVideoFilterRunnable
{
public:

    /*!
     * \fn QrRunnable(QrScannerFilter *filter)
     * \brief Constructor.
     * \param QrScannerFilter *filter - a pointer to filter.
     */
    QRRunnable(QrScannerFilter *filter)
    {
        _filter = filter;
    }

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

        if (_filter->getFutureThread().isRunning()) {
            return *input;
        }

        _filter->getDecoder()->setVideoFrame(*input);

        QImage captured = QrDecoder::videoFrameToImage(*input);
        _filter->getFutureThread() = QtConcurrent::run(processImage, _filter->getDecoder(), captured);

        return *input;
    }

private:
    QrScannerFilter *_filter;
};


QrScannerFilter::QrScannerFilter()
{
    _decoder = new QrDecoder;

    QObject::connect(_decoder, &QrDecoder::capturedChanged, this, &QrScannerFilter::setCaptured);
}

QVideoFilterRunnable *QrScannerFilter::createFilterRunnable()
{
    return new QRRunnable(this);
}

QString QrScannerFilter::captured() const
{
    return _captured;
}

void QrScannerFilter::setCaptured(const QString &newValue)
{
    if (newValue == _captured) {
        return;
    }

    _captured = newValue;

    emit capturedChanged();
}

QString QrScannerFilter::assayName() const
{
    return m_assayName;
}

QrDecoder *QrScannerFilter::getDecoder() const
{
    return _decoder;
}

bool QrScannerFilter::isDecoding() const
{
    return _isDecoding;
}

void QrScannerFilter::setAssayName(QString assayName)
{
    if (m_assayName == assayName) {
        return;
    }

    m_assayName = assayName;

    emit assayNameChanged(m_assayName);
}

QFuture<void> QrScannerFilter::getFutureThread() const
{
    return _futureThread;
}
