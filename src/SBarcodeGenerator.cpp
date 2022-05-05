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
              ZXing::ToMatrix<uint8_t>(writer.encode(ZXing::TextUtfEncoding::FromUtf8(inputString.toStdString()),
                m_width, m_height));

            m_filePath = QDir::tempPath() + "/" + m_fileName + "." + m_extension;

            if (m_extension == "png") {
                stbi_write_png(m_filePath.toStdString().c_str(), _bitmap.width(), _bitmap.height(), 1, _bitmap.data(),
                  0);
            } else if (m_extension == "jpg" || m_extension == "jpeg") {
                stbi_write_jpg(m_filePath.toStdString().c_str(), _bitmap.width(), _bitmap.height(), 1, _bitmap.data(),
                  0);
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
