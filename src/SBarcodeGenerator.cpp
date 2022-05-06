#include "SBarcodeGenerator.h"
#include <QStandardPaths>
#ifdef Q_OS_ANDROID
# include <QtAndroid>
#endif
#include "MultiFormatWriter.h"
#include "TextUtfEncoding.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

SBarcodeGenerator::SBarcodeGenerator(QQuickItem *parent)
    : QQuickItem(parent)
{ }

bool SBarcodeGenerator::generate(const QString &inputString)
{
    try {
        if (inputString.isEmpty()) {
            return false;
        } else {
            ZXing::MultiFormatWriter writer = ZXing::MultiFormatWriter(SCodes::toZXingFormat(m_format)).setMargin(
                m_margin).setEccLevel(m_eccLevel);


            _bitmap =
              ZXing::ToMatrix<int>(writer.encode(ZXing::TextUtfEncoding::FromUtf8(inputString.toStdString()),
                m_width, m_height) ,  m_background.rgba(), m_foreground.rgba());

            m_filePath = QDir::tempPath() + "/" + m_fileName + "." + m_extension;


            if (m_extension == "png") {
                stbi_write_png(m_filePath.toStdString().c_str(), _bitmap.width(), _bitmap.height(), 4, _bitmap.data(),
                   _bitmap.width() * 4);
            } else if (m_extension == "jpg" || m_extension == "jpeg") {
                stbi_write_jpg(m_filePath.toStdString().c_str(), _bitmap.width(), _bitmap.height(), 4, _bitmap.data(),
                   _bitmap.width() * 4);

            }

            emit generationFinished();

            return true;
        }
    } catch (const std::exception &e) {
        emit generationFinished(e.what());
    } catch (...) {
        emit generationFinished("Unsupported exception thrown");
    }

    return false;
} // SBarcodeGenerator::generate

bool SBarcodeGenerator::saveImage()
{
    if (m_filePath.isEmpty()) {
        return false;
    }

    #ifdef Q_OS_ANDROID
    if (QtAndroid::checkPermission(QString("android.permission.WRITE_EXTERNAL_STORAGE")) ==
      QtAndroid::PermissionResult::Denied)
    {
        QtAndroid::PermissionResultMap resultHash =
          QtAndroid::requestPermissionsSync(QStringList({ "android.permission.WRITE_EXTERNAL_STORAGE" }));
        if (resultHash["android.permission.WRITE_EXTERNAL_STORAGE"] == QtAndroid::PermissionResult::Denied) {
            return false;
        }
    }
    #endif

    QString docFolder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + m_fileName + "."
      + m_extension;

    QFile::copy(m_filePath, docFolder);

    return true;
}

const QColor &SBarcodeGenerator::background() const
{
    return m_background;
}

void SBarcodeGenerator::setBackground(const QColor &newBackground)
{
    if (m_background == newBackground)
        return;
    m_background = newBackground;
    emit backgroundChanged();
}

const QColor &SBarcodeGenerator::foreground() const
{
    return m_foreground;
}

void SBarcodeGenerator::setForeground(const QColor &newForeground)
{
    if (m_foreground == newForeground)
        return;
    m_foreground = newForeground;

    emit foregroundChanged();
}

SCodes::SBarcodeFormat SBarcodeGenerator::format() const
{
    return m_format;
}

void SBarcodeGenerator::setFormat(SCodes::SBarcodeFormat format)
{
    if (m_format != format) {
        switch (format) {
            case SCodes::SBarcodeFormat::None:
                qWarning() << "You need to set a specific format";
                return;

            case SCodes::SBarcodeFormat::Any:
            case SCodes::SBarcodeFormat::OneDCodes:
            case SCodes::SBarcodeFormat::TwoDCodes:
                qWarning() << "Multiple formats can't be used to generate a barcode";
                return;

            default:
                m_format = format;
                emit formatChanged(m_format);
        }
    }
}

void SBarcodeGenerator::setFormat(const QString &formatName)
{
    setFormat(SCodes::fromString(formatName));
}
