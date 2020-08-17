#include "QRScannerFilter.h"
#include "QRDecoder.h"

#include <QVideoFilterRunnable>
#include <QFuture>

#include <QtMultimedia/qvideoframe.h>
#include <QImage>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QDebug>

void processImage(QRDecoder *decoder, const QImage &image)
{
  decoder->process(image);
};

/// This is linked to the private implementations of Qt. Check, when using a new versino of Qt that it still
/// works. E.g. by saving the images to disk and visually inspect them
extern QImage qt_imageFromVideoFrame(const QVideoFrame &frame);
class QRRunnable : public QVideoFilterRunnable
{

public:

    QRRunnable( QRScannerFilter *filter)
    {
        _filter = filter;
    }

    QVideoFrame run(QVideoFrame *input,
                    const QVideoSurfaceFormat &surfaceFormat,
                    QVideoFilterRunnable::RunFlags flags) override
    {
        Q_UNUSED(surfaceFormat);
        Q_UNUSED(flags);

        if(_filter->getDecoder()->isDecoding()) {
            return *input;
        }

        if(_filter->getFutureThread().isRunning()) {
            return *input;
        }

        _filter->getDecoder()->setVideoFrame(*input);

        QImage captured = QRDecoder::videoFrameToImage(*input);
        _filter->getFutureThread() = QtConcurrent::run(processImage, _filter->getDecoder(), captured);

        return *input;
    }

private:
    QRScannerFilter *_filter;
};


QRScannerFilter::QRScannerFilter()
{

    _decoder = new QRDecoder;

    QObject::connect(_decoder, &QRDecoder::capturedChanged, this, &QRScannerFilter::setCaptured);
}

QVideoFilterRunnable *QRScannerFilter::createFilterRunnable()
{
    return new QRRunnable(this);
}

QString QRScannerFilter::captured()
{
    return _captured;
}

void QRScannerFilter::setCaptured(const QString &newValue)
{
    if(newValue == _captured)
    {
        return;
    }

    _captured = newValue;
    emit capturedChanged();
}

QString QRScannerFilter::assayName() const
{
    return m_assayName;
}

QRDecoder *QRScannerFilter::getDecoder() const
{
    return _decoder;
}

bool QRScannerFilter::isDecoding() const
{
    return _isDecoding;
}

void QRScannerFilter::setAssayName(QString assayName)
{
    if (m_assayName == assayName) {
        return;
    }

    m_assayName = assayName;
    emit assayNameChanged(m_assayName);
}

QFuture<void> QRScannerFilter::getFutureThread() const
{
    return _futureThread;
}
