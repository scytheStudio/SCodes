#include "SBarcodeGenerator.h"
#include <QStandardPaths>
#include <QPainter>

#ifdef Q_OS_ANDROID
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    #include <QtAndroid>
#else
    #include <QtCore/private/qandroidextras_p.h>
#endif
#endif

#include "MultiFormatWriter.h"

SBarcodeGenerator::SBarcodeGenerator(QQuickItem *parent)
    : QQuickItem(parent)
{ }

bool SBarcodeGenerator::generate(const QString &inputString)
{
    try {
        if (inputString.isEmpty()) {
            return false;
        } else {
            // Change ecc level to max to generate image on QR code.
            if (m_format == SCodes::SBarcodeFormat::QRCode && !m_imagePath.isEmpty()) {
                if (m_eccLevel < 8) {
                    qDebug() << "To draw image on QR Code use maximum level of ecc. Setting it to 8.";

                    setEccLvel(8);
                }
            }

            ZXing::MultiFormatWriter writer = ZXing::MultiFormatWriter(SCodes::toZXingFormat(m_format))
                                                  .setMargin(m_margin)
                                                  .setEccLevel(m_eccLevel);

            auto qrCodeMatrix = writer.encode(inputString.toStdString(), m_width, m_height);

            QImage image(m_width, m_height, QImage::Format_ARGB32);

            for (int y = 0; y < m_height; ++y) {
                for (int x = 0; x < m_width; ++x) {
                    if (qrCodeMatrix.get(x, y)) {
                        image.setPixelColor(x, y, m_foregroundColor);
                    } else {
                        image.setPixelColor(x, y, m_backgroundColor);
                    }
                }
            }

            // Center images works only on QR codes.
            if (m_format == SCodes::SBarcodeFormat::QRCode) {
                if (!m_imagePath.isEmpty()) {
                    QSize centerImageSize(m_width / m_centerImageRatio, m_height / m_centerImageRatio);
                    drawCenterImage(&image, m_imagePath, centerImageSize,
                                    (image.width() - centerImageSize.width()) / 2,
                                    (image.height() - centerImageSize.height()) / 2);
                } else {
                    qDebug() << "Center Image path is empty. Skip drawing center image.";
                }
            } else {
                qDebug() << "Center images works only on QR codes.";
            }

            m_filePath = QDir::tempPath() + "/" + m_fileName + "." + m_extension;

            QFile file{m_filePath};
            if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                image.save(&file);
            } else {
                qWarning() << "Could not open file for writing!";
                return false;
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

    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    if (QtAndroid::checkPermission(QString("android.permission.WRITE_EXTERNAL_STORAGE")) ==
      QtAndroid::PermissionResult::Denied)
    {
        QtAndroid::PermissionResultMap resultHash =
          QtAndroid::requestPermissionsSync(QStringList({ "android.permission.WRITE_EXTERNAL_STORAGE" }));
        if (resultHash["android.permission.WRITE_EXTERNAL_STORAGE"] == QtAndroid::PermissionResult::Denied) {
            return false;
        }
    }
    #else

    QtAndroidPrivate::requestPermission(QString("android.permission.WRITE_EXTERNAL_STORAGE"));

    #endif
    #endif

    QString docFolder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + m_fileName + "."
      + m_extension;

    QFile::copy(m_filePath, docFolder);

    return true;
}

void SBarcodeGenerator::drawCenterImage(QImage *parentImage, const QString &imagePath, QSize imageSize, int x, int y)
{
    QImage centerImage(imageSize, QImage::Format_RGB32);
    centerImage.load(imagePath);

    if (centerImage.isNull()) {
        qWarning() << "Center image could not be loaded!";
        return;
    }

    // Create a painter to overlay the center image on the parentImage.
    QPainter painter(parentImage);

    // Draw background rectangle.
    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.drawRect(x, y, imageSize.width(), imageSize.height());

    // Scale the center image to be smaller than background rectangle.
    float imageRatio = 0.8;
    centerImage = centerImage.scaled(imageSize.width() * imageRatio,
                                     imageSize.height() * imageRatio,
                                     Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Draw image.
    painter.drawImage(x + (imageSize.width() - centerImage.width()) / 2,
                      y + (imageSize.height() - centerImage.height()) / 2,
                      centerImage);
    painter.end();
}

void SBarcodeGenerator::setEccLvel(int eccLevel)
{
    if (m_eccLevel == eccLevel) {
        return;
    }

    m_eccLevel = eccLevel;
    emit eccLevelChanged(m_eccLevel);
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

QString SBarcodeGenerator::imagePath() const
{
    return m_imagePath;
}

void SBarcodeGenerator::setImagePath(const QString &imagePath)
{
    if (m_imagePath == imagePath) {
        return;
    }

    m_imagePath = imagePath;
    emit imagePathChanged();
}

int SBarcodeGenerator::centerImageRatio() const
{
    return m_centerImageRatio;
}

void SBarcodeGenerator::setCenterImageRatio(int centerImageRatio)
{
    if (m_centerImageRatio == centerImageRatio) {
        return;
    }

    m_centerImageRatio = centerImageRatio;
    emit centerImageRatioChanged();
}

QColor SBarcodeGenerator::foregroundColor() const
{
    return m_foregroundColor;
}

void SBarcodeGenerator::setForegroundColor(const QColor &foregroundColor)
{
    if (m_foregroundColor == foregroundColor) {
        return;
    }

    m_foregroundColor = foregroundColor;
    emit foregroundColorChanged();
}

QColor SBarcodeGenerator::backgroundColor() const
{
    return m_backgroundColor;
}

void SBarcodeGenerator::setBackgroundColor(const QColor &backgroundColor)
{
    if (m_backgroundColor == backgroundColor) {
        return;
    }

    m_backgroundColor = backgroundColor;
    emit backgroundColorChanged();
}
