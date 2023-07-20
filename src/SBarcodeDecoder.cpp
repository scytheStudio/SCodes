#include "SBarcodeDecoder.h"

#include <QDebug>
#include <QImage>
#include <QtMultimedia/qvideoframe.h>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOffscreenSurface>
#include <iostream>

#include <ReadBarcode.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


/*!
 * \brief Provide a interface to access `ZXing::ReadBarcode` method
 */
namespace ZXing {
namespace Qt {
using ZXing::DecodeHints;
using ZXing::BarcodeFormat;
using ZXing::BarcodeFormats;
using ZXing::Binarizer;

template <typename T, typename _ = decltype(ToString(T()))>
QDebug operator << (QDebug dbg, const T& v)
{
    return dbg.noquote() << QString::fromStdString(ToString(v));
}

/*!
 * \brief Encapsulates the result of decoding a barcode within an image
 */
class Result : private ZXing::Result
{
public:
    /*!
     * \fn explicit Result(ZXing::Result&& r) : ZXing::Result(std::move(r))
     * \brief Constructor
     * \param ZXing::Result&& r - Result class
     */
    explicit Result(ZXing::Result&& r) : ZXing::Result(std::move(r)){ }

    using ZXing::Result::format;
    using ZXing::Result::isValid;
    using ZXing::Result::status;

    /*!
     * \fn inline QString text() const
     * \return scanned result human readable text
     */
    inline QString text() const { return QString::fromWCharArray(ZXing::Result::text().c_str()); }
};

/*!
 * \fn Result ReadBarcode(const QImage& img, const DecodeHints& hints = { })
 * \brief Interface for calling ZXing::ReadBarcode method to get result as a text.
 * \param const QImage& img - referance of the image to be processed
 * \param const DecodeHints& hints - barcode decode hints
 */
Result ReadBarcode(const QImage& img, const DecodeHints& hints = { })
{
    auto ImgFmtFromQImg = [](const QImage& img){
          switch (img.format()) {
              case QImage::Format_ARGB32:
              case QImage::Format_RGB32:
                  #if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
                  return ImageFormat::BGRX;

                  #else
                  return ImageFormat::XRGB;

                  #endif
              case QImage::Format_RGB888: return ImageFormat::RGB;

              case QImage::Format_RGBX8888:
              case QImage::Format_RGBA8888: return ImageFormat::RGBX;

              case QImage::Format_Grayscale8: return ImageFormat::Lum;

              default: return ImageFormat::None;
          }
      };

    auto exec = [&](const QImage& img){
          return Result(ZXing::ReadBarcode({ img.bits(), img.width(), img.height(), ImgFmtFromQImg(img) }, hints));
      };

    return ImgFmtFromQImg(img) == ImageFormat::None ? exec(img.convertToFormat(QImage::Format_RGBX8888)) : exec(img);
}
} // Qt namespace
} // ZXing namespace

using namespace ZXing::Qt;

std::ostream& operator << (std::ostream& os, const std::vector<ZXing::ResultPoint>& points)
{
    for (const auto& p : points) {
        os << int(p.x() + .5f) << "x" << int(p.y() + .5f) << " ";
    }

    return os;
}

static int m_resolutionWidth = DEFAULT_RES_W;
static int m_resolutionHeight = DEFAULT_RES_H;

SBarcodeDecoder::SBarcodeDecoder(QObject *parent) : QObject(parent)
{ }

void SBarcodeDecoder::clean()
{
    m_captured = "";
}

QString SBarcodeDecoder::captured() const
{
    return m_captured;
}

void SBarcodeDecoder::setCaptured(const QString &captured)
{
    if (m_captured == captured) {
        return;
    }

    m_captured = captured;

    emit capturedChanged(m_captured);
}

void SBarcodeDecoder::setIsDecoding(bool isDecoding)
{
    if (m_isDecoding == isDecoding) {
        return;
    }

    m_isDecoding = isDecoding;

    emit isDecodingChanged(m_isDecoding);
}

bool SBarcodeDecoder::  isDecoding() const
{
    return m_isDecoding;
}

void SBarcodeDecoder::process(const QImage capturedImage, ZXing::BarcodeFormats formats)
{
    setIsDecoding(true);

    const auto hints = DecodeHints()
      .setFormats(formats)
      .setTryHarder(true)
      .setTryRotate(true)
      .setIsPure(false)
      .setBinarizer(Binarizer::LocalAverage);

    const auto result = ReadBarcode(capturedImage, hints);

    if (result.isValid()) {
        setCaptured(result.text());
    }

    setIsDecoding(false);
}

QImage SBarcodeDecoder::videoFrameToImage(const QVideoFrame &videoFrame, const QRect &captureRect)
{

    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

    auto handleType = videoFrame.handleType();

    if (handleType == QAbstractVideoBuffer::NoHandle) {
        #if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
        QImage image = videoFrame.image();
        #else

        videoFrame.map(QAbstractVideoBuffer::ReadOnly);
        QImage image = imageFromVideoFrame(videoFrame);
        videoFrame.unmap();

        #endif

        if (image.isNull()) {
            return QImage();
        }

        if (image.format() != QImage::Format_ARGB32) {
            image = image.convertToFormat(QImage::Format_ARGB32);
        }

        return image.copy(captureRect);
    }

    if (handleType == QAbstractVideoBuffer::GLTextureHandle) {
        QImage image(videoFrame.width(), videoFrame.height(), QImage::Format_ARGB32);

        GLuint textureId = static_cast<GLuint>(videoFrame.handle().toInt());

        QOpenGLContext *ctx = QOpenGLContext::currentContext();

        QOpenGLFunctions *f = ctx->functions();

        GLuint fbo;

        f->glGenFramebuffers(1, &fbo);

        GLint prevFbo;

        f->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
        f->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
        f->glReadPixels(0, 0, videoFrame.width(), videoFrame.height(), GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
        f->glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>( prevFbo ) );

        return image.rgbSwapped().copy(captureRect);
    }

    #else
    // The CPU / GPU buffer check is done internally, or so it seems
    QImage image = videoFrame.toImage();

    if (image.isNull()) {
        return QImage();
    }

    if (image.format() != QImage::Format_ARGB32) {
        image = image.convertToFormat(QImage::Format_ARGB32);
    }

    // that's because qml videooutput has no mapNormalizedRectToItem method
#ifdef Q_OS_ANDROID
    return image.copy(m_resolutionHeight/4, m_resolutionWidth/4, m_resolutionHeight/2, m_resolutionWidth/2);
#else
    return image.copy(captureRect);
#endif // Q_OS_ANDROID


#endif // QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

}

void SBarcodeDecoder::setResolution(const int &w, const int &h)
{
    m_resolutionWidth = w;
    m_resolutionHeight = h;
} // SBarcodeDecoder::videoFrameToImage
