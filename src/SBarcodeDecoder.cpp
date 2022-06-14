#include "SBarcodeDecoder.h"

#include <QDebug>
#include <QImage>
#include <QtMultimedia/qvideoframe.h>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <iostream>

#include <ReadBarcode.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//#include "qvideoframeconversionhelper_p.h"

#include <QOffscreenSurface>

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

class Result : private ZXing::Result
{
public:
    explicit Result(ZXing::Result&& r) : ZXing::Result(std::move(r)){ }

    using ZXing::Result::format;
    using ZXing::Result::isValid;
    using ZXing::Result::status;

    inline QString text() const { return QString::fromWCharArray(ZXing::Result::text().c_str()); }
};

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
        qDebug() << result.text();
    }

    setIsDecoding(false);
}

QImage SBarcodeDecoder::videoFrameToImage(const QVideoFrame &videoFrame, const QRect &captureRect)
{

    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

    auto handleType = videoFrame.handleType();

    switch (handleType) {
        case QAbstractVideoBuffer::NoHandle: {

            QImage image = videoFrame.image();

            //videoFrame.map(QAbstractVideoBuffer::ReadOnly);
            //QImage image = imageFromVideoFrame(videoFrame);
            //videoFrame.unmap();

            if (image.isNull()) {
                return QImage();
            }

            if (image.format() != QImage::Format_ARGB32) {
                image = image.convertToFormat(QImage::Format_ARGB32);
            }

            return image.copy(captureRect);

        } break;

        case QAbstractVideoBuffer::GLTextureHandle: {

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

        } break;
    }

    #else

    auto handleType = videoFrame.handleType();

    switch (handleType) {
        case QVideoFrame::NoHandle: {

            QImage image = videoFrame.toImage();

            if (image.isNull()) {
                return QImage();
            }

            if (image.format() != QImage::Format_ARGB32) {
                image = image.convertToFormat(QImage::Format_ARGB32);
            }

            return image.copy(captureRect);

        } break;
        case QVideoFrame::RhiTextureHandle: { // NOP, videoFrame.handleType always returns NoHandle

            QImage image(videoFrame.width(), videoFrame.height(), QImage::Format_ARGB32);

            GLuint textureId = static_cast<GLuint>(1); // handle type enum 1 #videoFrame.handle().toInt()

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

        } break;
    }

    #endif

    return QImage();
} // SBarcodeDecoder::videoFrameToImage


/*
QImage SBarcodeDecoder::imageFromVideoFrame(const QVideoFrame &videoFrame)
{
    uchar *ARGB32Bits = new uchar[(videoFrame.width() * videoFrame.height()) * 4];

    QImage::Format imageFormat = videoFrame.imageFormatFromPixelFormat(videoFrame.pixelFormat());

    qDebug() << "IMAGE FORMAT: " << imageFormat;

    if (imageFormat == QImage::Format_Invalid) {
        switch (videoFrame.pixelFormat()) {
            case QVideoFrame::Format_YUYV: qt_convert_YUYV_to_ARGB32(videoFrame, ARGB32Bits);
                break;
            case QVideoFrame::Format_NV12: qt_convert_NV12_to_ARGB32(videoFrame, ARGB32Bits);
                break;
            case QVideoFrame::Format_YUV420P: qt_convert_YUV420P_to_ARGB32(videoFrame, ARGB32Bits);
                break;
            case QVideoFrame::Format_YV12: qt_convert_YV12_to_ARGB32(videoFrame, ARGB32Bits);
                break;
            case QVideoFrame::Format_AYUV444: qt_convert_AYUV444_to_ARGB32(videoFrame, ARGB32Bits);
                break;
            case QVideoFrame::Format_YUV444: qt_convert_YUV444_to_ARGB32(videoFrame, ARGB32Bits);
                break;
            case QVideoFrame::Format_UYVY: qt_convert_UYVY_to_ARGB32(videoFrame, ARGB32Bits);
                break;
            case QVideoFrame::Format_NV21: qt_convert_NV21_to_ARGB32(videoFrame, ARGB32Bits);
                break;
            case QVideoFrame::Format_BGRA32: qt_convert_BGRA32_to_ARGB32(videoFrame, ARGB32Bits);
                break;
            case QVideoFrame::Format_BGR24: qt_convert_BGR24_to_ARGB32(videoFrame, ARGB32Bits);
                break;
            case QVideoFrame::Format_BGR565: qt_convert_BGR565_to_ARGB32(videoFrame, ARGB32Bits);
                break;
            case QVideoFrame::Format_BGR555: qt_convert_BGR555_to_ARGB32(videoFrame, ARGB32Bits);
                break;
            default: break;
        }

        return QImage(ARGB32Bits,
                 videoFrame.width(),
                 videoFrame.height(),
                 QImage::Format_ARGB32);
    }

    return QImage(videoFrame.bits(),
             videoFrame.width(),
             videoFrame.height(),
             imageFormat);
}*/ // SBarcodeDecoder::imageFromVideoFrame
